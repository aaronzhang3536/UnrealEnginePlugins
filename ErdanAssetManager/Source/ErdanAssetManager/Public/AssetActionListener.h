#pragma once

#include "CoreMinimal.h"
#include "AssetRegistry/AssetRegistryModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAssetActionListener, Log, All);

class FAssetActionListener
{
public:
	FAssetActionListener();
	~FAssetActionListener();

	/** 注册资源事件监听器 */
	void RegisterAssetEventHandlers();

	/** 注销资源事件监听器 */
	void UnregisterAssetEventHandlers();

private:
	void InitRuleDatas();

	/** 资源添加事件回调 */
	void OnAssetAdded(const FAssetData& AssetData);

	/** 资源移除事件回调 */
	void OnAssetRemoved(const FAssetData& AssetData);

	/** 资源重命名事件回调 */
	void OnAssetRenamed(const FAssetData& AssetData, const FString& OldPath);

	/* 资源加载事件回调 */
	void OnAssetLoaded(UObject* LoadedObject);

	/* 资源保存前事件回调 */
	void OnAssetPreSave(UObject* Asset);

	/* 资源保存后事件回调 */
	void OnAssetPostSave(UObject* Asset);

	FDelegateHandle OnAssetAddedHandle;
	FDelegateHandle OnAssetRemovedHandle;
	FDelegateHandle OnAssetRenamedHandle;
	FDelegateHandle OnAssetLoadedHandle;
	FDelegateHandle OnAssetPreSaveHandle;
	FDelegateHandle OnAssetPostSaveHandle;

private:
	TArray<TObjectPtr<class UErdanAssetManagerRuleData>> RuleDatas;
	TMap<FDirPath, FSinglePathRules> FinalRules;

};
