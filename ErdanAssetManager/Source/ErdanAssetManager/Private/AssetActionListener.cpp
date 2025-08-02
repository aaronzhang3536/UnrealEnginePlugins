#include "AssetActionListener.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/MessageDialog.h"
#include "ErdanAssetManagerRuleData.h"
#include "PropertyPathHelpers.h"

DEFINE_LOG_CATEGORY(LogAssetActionListener);

FAssetActionListener::FAssetActionListener()
{
	InitRuleDatas();
	RegisterAssetEventHandlers();
}

FAssetActionListener::~FAssetActionListener()
{
	UnregisterAssetEventHandlers();
}

void FAssetActionListener::InitRuleDatas()
{
	ITargetPlatformManagerModule& PlatformManager = GetTargetPlatformManagerRef();
	const TArray<ITargetPlatform*>& ActivePlatforms = PlatformManager.GetActiveTargetPlatforms();
	TArray<FString> ActivePlatformNames;
	for (const ITargetPlatform* Platform : ActivePlatforms)
	{
		ActivePlatformNames.Add(Platform->PlatformName());
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	FARFilter Filter;
	Filter.ClassPaths.Add(FTopLevelAssetPath(UErdanAssetManagerRuleData::StaticClass()->GetPathName()));
	Filter.bRecursiveClasses = true;

	
	TArray<FAssetData> RuleDataAssets;
	AssetRegistryModule.Get().GetAssets(Filter, RuleDataAssets);

	for (FAssetData& Asset : RuleDataAssets)
	{
		TObjectPtr <UErdanAssetManagerRuleData> RuleData = Cast<UErdanAssetManagerRuleData>(Asset.GetAsset());
		if (RuleData && ActivePlatformNames.Contains(RuleData->TargetPlatform))
		{
			RuleDatas.Add(RuleData);
		}
	}

	RuleDatas.Sort([](const TObjectPtr<UErdanAssetManagerRuleData>& A, const TObjectPtr<UErdanAssetManagerRuleData>& B)
		{
			if (A && B)
			{
				auto ExtractIndex = [](const FString& Name) -> int32
					{
						int32 UnderscoreIndex;
						if (Name.FindLastChar('_', UnderscoreIndex))
						{
							FString NumberPart = Name.Mid(UnderscoreIndex + 1);
							return FCString::Atoi(*NumberPart);
						}
						return -1; // 若无下划线或无数字，返回-1
					};

				int32 AIndex = ExtractIndex(A->GetFName().ToString());
				int32 BIndex = ExtractIndex(B->GetFName().ToString());

				return AIndex < BIndex; // 升序
			}

			//无效的放前面
			if (!A) return false;
			if (!B) return true;
			return false;
		});

	///// 保留支持多个RuleData的逻辑,但目前实际只支持一个

	TObjectPtr<UErdanAssetManagerRuleData> ActiveRuleData = RuleDatas.Num() > 0 ? RuleDatas[0] : nullptr;
	RuleDatas.Empty();
	RuleDatas.Add(ActiveRuleData);

	for (TObjectPtr<UErdanAssetManagerRuleData> RuleData : RuleDatas)
	{
		if(RuleData)
			FinalRules.Append(RuleData->ManagedPathsRules);
	}

	//根据路径长短再进行排序
	FinalRules.KeySort([](const FDirPath& InFirst, const FDirPath& InSecond) -> bool
	{
		return InFirst.Path < InSecond.Path;
	});
}

void FAssetActionListener::RegisterAssetEventHandlers()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	OnAssetAddedHandle = AssetRegistryModule.Get().OnAssetAdded().AddRaw(this, &FAssetActionListener::OnAssetAdded);
	OnAssetRemovedHandle = AssetRegistryModule.Get().OnAssetRemoved().AddRaw(this, &FAssetActionListener::OnAssetRemoved);
	OnAssetRenamedHandle = AssetRegistryModule.Get().OnAssetRenamed().AddRaw(this, &FAssetActionListener::OnAssetRenamed);
}

void FAssetActionListener::UnregisterAssetEventHandlers()
{
	if (FModuleManager::Get().IsModuleLoaded("AssetRegistry"))
	{
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::GetModuleChecked<FAssetRegistryModule>("AssetRegistry");

		AssetRegistryModule.Get().OnAssetAdded().Remove(OnAssetAddedHandle);
		AssetRegistryModule.Get().OnAssetRemoved().Remove(OnAssetRemovedHandle);
		AssetRegistryModule.Get().OnAssetRenamed().Remove(OnAssetRenamedHandle);
	}
}

void FAssetActionListener::OnAssetAdded(const FAssetData& AssetData)
{
	for (auto It = FinalRules.CreateConstIterator(); It; ++It) 
	{
		if (AssetData.GetSoftObjectPath().ToString().Contains(*It->Key.Path))
		{
			const FAssetRuleList* RuleList = It.Value().TypeRules.Find(AssetData.GetClass());
			if (RuleList)
			{
				for (const FAssetRuleItem& RuleItem : RuleList->Rules)
				{
					FString AssetName = AssetData.AssetName.ToString();
					FRegexPattern Pattern(RuleItem.RegularExpression);
					FRegexMatcher Matcher(Pattern, AssetName);
					if (Matcher.FindNext())
					{
						TObjectPtr<UObject> Object = AssetData.GetAsset();
						if (Object)
						{
							for (const FString& PropertyPath : RuleItem.ManagedPropertyPath)
							{
								//FProperty* foundProperty = nullptr;
								//if (FemaleObject)
								//{
								//	for (UClass* TempClass = FemaleObject->GetClass(); TempClass; TempClass = TempClass->GetSuperClass())
								//	{
								//		foundProperty = FindFProperty<FProperty>(TempClass, *CachedPathStr);
								//		if (foundProperty)
								//		{
								//			break;
								//		}
								//	}
								//}
								//if (foundProperty)

								FCachedPropertyPath SrcCachedPropertyPath(PropertyPath);
								SrcCachedPropertyPath.SetCachedContainer(RuleItem.RuleObject);
								FCachedPropertyPath DstCachedPropertyPath(PropertyPath);
								DstCachedPropertyPath.SetCachedContainer(Object);								
								PropertyPathHelpers::CopyPropertyValue(Object, DstCachedPropertyPath, SrcCachedPropertyPath);
							}
						}
					}

				}
			}			
		}
	}
	UE_LOG(LogAssetActionListener, Log, TEXT("Asset Added: %s"), *AssetData.AssetName.ToString());
}

void FAssetActionListener::OnAssetRemoved(const FAssetData& AssetData)
{
	UE_LOG(LogAssetActionListener, Log, TEXT("Asset Removed: %s"), *AssetData.AssetName.ToString());
}

void FAssetActionListener::OnAssetRenamed(const FAssetData& AssetData, const FString& OldPath)
{
	UE_LOG(LogAssetActionListener, Log, TEXT("Asset Renamed: %s -> %s"), *OldPath, *AssetData.ObjectPath.ToString());
}


void FAssetActionListener::OnAssetLoaded(UObject* LoadedObject)
{
	if (LoadedObject)
	{
		UE_LOG(LogAssetActionListener, Log, TEXT("[Asset Loaded] %s"), *LoadedObject->GetPathName());
	}
}

void FAssetActionListener::OnAssetPreSave(UObject* Asset)
{
	if (Asset)
	{
		UE_LOG(LogAssetActionListener, Log, TEXT("[Asset PreSave] %s"), *Asset->GetPathName());
	}
}

void FAssetActionListener::OnAssetPostSave(UObject* Asset)
{
	if (Asset)
	{
		UE_LOG(LogAssetActionListener, Log, TEXT("[Asset PostSave] %s"), *Asset->GetPathName());
	}
}
