// Copyright Epic Games, Inc. All Rights Reserved.

#include "SErdanAssetManagerView.h"
#include "DetailsViewArgs.h"
#include "Misc/FeedbackContext.h"
#include "IDetailsView.h"
#include "Misc/ScopedSlowTask.h"
#include "Misc/PackageName.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableComboBox.h"
#include "Framework/Docking/TabManager.h"
#include "CanvasItem.h"

#include "PropertyEditorModule.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

#include "ErdanAssetManagerDetailCustomization.h"
#include "ErdanAssetManagerRuleData.h"
#include "ErdanAssetManagerRuleTreeColumn.h"
#define LOCTEXT_NAMESPACE "SErdanAssetManagerView"
#define PLUG_PKG_NAME "/ErdanAssetManager"
void SErdanAssetManagerView::Construct(const FArguments& InArgs)
{
	InitRuleData();

	ForceRefreshManagedPath();
	CreateManagedPathComboBox();

	ForceRefreshTypeRules();
	SetupTreeViewColumns();
	CreateTypeRulesTreeView();

	CreatePropertyView();

	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(FAppStyle::GetBrush("DetailsView.CategoryTop"))
		.Padding(FMargin(0.0f, 3.0f, 1.0f, 0.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.0f)
					[
						ManagedPathComboBox.ToSharedRef()
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
							.Padding(2.0f,2.0f,1.0f,2.0f)
							[
								SAssignNew(AddManagedPath_BT, SButton)
									.OnClicked(this, &SErdanAssetManagerView::OnAddManagedPath)
									.ToolTipText(LOCTEXT("AddManagedPathToolTipText", "Choose a path"))
									.ContentPadding(2.0f)
									.ForegroundColor(FSlateColor::UseForeground())
									.IsFocusable(false)
									//.Text(LOCTEXT("AddManagedPath", "Add"))
									[
										SNew(SImage)
											.Image(FAppStyle::Get().GetBrush("Icons.Plus"))
											.ColorAndOpacity(FStyleColors::AccentGreen)
									]
							]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
							.Padding(1.0f, 2.0f, 1.0f, 2.0f)
							[
								SNew(SButton)
									.OnClicked(this, &SErdanAssetManagerView::OnDeleteManagedPath)
									.ToolTipText(LOCTEXT("DeleteManagedPathToolTipText", "Delete current path"))
									.ContentPadding(2.0f)
									.ForegroundColor(FSlateColor::UseForeground())
									.IsFocusable(false)
									//.Text(LOCTEXT("DeleteManagedPath", "Delete"))
									[
										SNew(SImage)
											.Image(FAppStyle::Get().GetBrush("Icons.Delete"))
											.ColorAndOpacity(FStyleColors::AccentRed)
									]
							]
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						SNew(SBox)
							.Padding(1.0f, 2.0f, 2.0f, 2.0f)
							[
								SNew(SButton)
									.OnClicked(this, &SErdanAssetManagerView::OnSaveRuleData)
									.ToolTipText(LOCTEXT("SaveRuleDataToolTipText", "Save rule data"))
									.ContentPadding(2.0f)
									.ForegroundColor(FSlateColor::UseForeground())
									.IsFocusable(false)
									//.Text(LOCTEXT("SaveManagedPath", "Save"))
									[
										SNew(SImage)
											.Image(FAppStyle::Get().GetBrush("Icons.Save"))
											.ColorAndOpacity(FStyleColors::Foreground)
									]
							]
					]
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0)
			[
				SNew(SSplitter)
				.Orientation(Orient_Horizontal)
				.ResizeMode(ESplitterResizeMode::FixedPosition)
					+ SSplitter::Slot()
					.Value(0.1)
					[
						TreeView.ToSharedRef()
					]
					+ SSplitter::Slot()
					.Value(0.9)
					[
						PropertyView.ToSharedRef()
					]
			]
		]
	];
}

TSharedRef<SWidget> SErdanAssetManagerView::OnGenerateManagedPathComboItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

void SErdanAssetManagerView::OnCurrentManagedPathChanged(TSharedPtr<FString> NewChoice, ESelectInfo::Type SelectType)
{
	ForceRefreshTypeRules();
}

FReply SErdanAssetManagerView::OnAddManagedPath()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	FPathPickerConfig PathPickerConfig;
	PathPickerConfig.DefaultPath = GetCurrentManagedPath();
	PathPickerConfig.bAllowContextMenu = false;
	PathPickerConfig.OnPathSelected = FOnPathSelected::CreateLambda([&](const FString& Path)
	{
		if (AddManagedPathPickerMenu.IsValid())
		{
			AddManagedPathPickerMenu->Dismiss();
			AddManagedPathPickerMenu.Reset();
		}
		if (RuleData.IsValid() && ManagedPathComboBox.IsValid())
		{
			TSharedPtr<FString> NewPath ;
			if (RuleData->ManagedPathsRules.Contains(Path))
			{
				for (TSharedPtr<FString>& ThePath : ManagedPaths)
				{
					if (ThePath.IsValid() && *ThePath.Get() == Path)
					{
						NewPath = ThePath;
						break;
					}
				}
			}
			else
			{
				NewPath = MakeShared<FString>(Path);
				RuleData->ManagedPathsRules.Add(Path);
				ManagedPaths.Add(NewPath);
			}
			ManagedPathComboBox->SetSelectedItem(NewPath);
		}
	});

	FMenuBuilder MenuBuilder(true, NULL);
	MenuBuilder.AddWidget(SNew(SBox)
		.WidthOverride(300.0f)
		.HeightOverride(300.0f)
		[
			ContentBrowserModule.Get().CreatePathPicker(PathPickerConfig)
		], FText());


	AddManagedPathPickerMenu = FSlateApplication::Get().PushMenu(AddManagedPath_BT.ToSharedRef(),
		FWidgetPath(),
		MenuBuilder.MakeWidget(),
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu)
	);


	return FReply::Handled();
}

FReply SErdanAssetManagerView::OnDeleteManagedPath()
{
	return FReply::Handled();
}

void SErdanAssetManagerView::CreatePropertyView()
{
	FPropertyEditorModule& EditModule = FModuleManager::Get().GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
	DetailsViewArgs.bShowPropertyMatrixButton = false;
	DetailsViewArgs.bShowScrollBar = true;
	PropertyView = EditModule.CreateDetailView(DetailsViewArgs);
	PropertyView->SetGenericLayoutDetailsDelegate(FOnGetDetailCustomizationInstance::CreateStatic(&FErdanAssetManagerDetailCustomization::MakeInstance));
}

void SErdanAssetManagerView::CreateManagedPathComboBox()
{
	SAssignNew(ManagedPathComboBox, SComboBox<TSharedPtr<FString>>)
		.ButtonStyle(FAppStyle::Get(), "PropertyEditor.AssetComboStyle")
		.InitiallySelectedItem(ManagedPaths.Num() > 0 ? ManagedPaths[0] : MakeShareable(new FString("")))
		.OptionsSource(&ManagedPaths)
		.OnGenerateWidget(this, &SErdanAssetManagerView::OnGenerateManagedPathComboItem)
		.OnSelectionChanged(this, &SErdanAssetManagerView::OnCurrentManagedPathChanged)
		.Content()
		[
			SNew(STextBlock).Text_Lambda([&]()->FText {return FText::FromString(GetCurrentManagedPath()); })
		];
}

void SErdanAssetManagerView::CreateTypeRulesTreeView()
{
	SAssignNew(TreeView, SErdanAssetManagerRuleTreeView, StaticCastSharedRef<SErdanAssetManagerView>(AsShared()))
		.TreeItemsSource(&TreeItems)
		.OnGenerateRow_Lambda([&, this](FAssetRuleTreeItemPtr InItem, const TSharedRef<STableViewBase>& OwnerTable)
		{
			return SNew(SAssetRuleTreeRow, TreeView.ToSharedRef(), SharedThis(this)).Item(InItem);
		})
		.SelectionMode(ESelectionMode::Single)
		.OnSelectionChanged_Lambda([&](FAssetRuleTreeItemPtr InItem, ESelectInfo::Type InSelectType)
		{
			if (PropertyView.IsValid() && InItem.IsValid() && InItem->RuleItem)
			{
				FAssetRuleTreeItemPtr OldMetaData = PropertyView->GetMetaData<FAssetRuleTreeItem>();
				if (OldMetaData.IsValid())
				{
					PropertyView->RemoveMetaData<FAssetRuleTreeItem>(OldMetaData.ToSharedRef());
				}
				PropertyView->SetObject(InItem->RuleItem->RuleObject);
				PropertyView->AddMetadata<FAssetRuleTreeItem>(InItem.ToSharedRef());
			}
		})
		.OnGetChildren_Lambda([&](FAssetRuleTreeItemPtr Item, TArray<FAssetRuleTreeItemPtr>& OutChildren)
		{
			OutChildren.Append(Item->Children);
		})
		.HeaderRow(RuleTreeViewHeaderRow);
}

void SErdanAssetManagerView::InitRuleData()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	RuleData = NewObject<UErdanAssetManagerRuleData>(GetTransientPackage(), NAME_None, RF_Transient | RF_Standalone);
	TArray<FAssetData> AssetDatas;
	FARFilter Filter;
	Filter.ClassPaths.Add(FTopLevelAssetPath(*UErdanAssetManagerRuleData::StaticClass()->GetPathName()));
	Filter.bRecursivePaths = true;
	Filter.bRecursiveClasses = true;
	Filter.PackagePaths.Add(FName(PLUG_PKG_NAME));
	Filter.PackagePaths.Add(FName("/Content"));
	if (AssetRegistryModule.Get().GetAssets(Filter, AssetDatas, true))
	{
		for (const auto& AssetData : AssetDatas)
		{
			UErdanAssetManagerRuleData* TempRuleData = Cast<UErdanAssetManagerRuleData>(AssetData.GetAsset());

			if (TempRuleData && RuleData.IsValid())
			{
				RuleData->ManagedPathsRules.Append(TempRuleData->ManagedPathsRules);
			}
		}
	}
}

void SErdanAssetManagerView::ForceRefreshManagedPath()
{
	ManagedPaths.Empty();
	if (RuleData.IsValid())
	{
		TArray<FString> Keys;
		RuleData->ManagedPathsRules.GetKeys(Keys);
		for (const FString& ManagedPath : Keys)
		{
			ManagedPaths.Emplace(new FString(ManagedPath));
		}
	}
}

void SErdanAssetManagerView::ForceRefreshTypeRules()
{
	TreeItems.Empty();
	FString ManagedPath = GetCurrentManagedPath();
	if (!ManagedPath.IsEmpty())
	{
		if (RuleData->ManagedPathsRules.Contains(ManagedPath))
		{
			FSinglePathRules& PathRules = *RuleData->ManagedPathsRules.Find(ManagedPath);
			for (TPair<UClass*, FAssetRuleItem>& SingleTypeRule : PathRules.TypeRules)
			{
				FString TypeName = SingleTypeRule.Key != nullptr ? SingleTypeRule.Key->GetPathName() : FString();
				FAssetRuleTreeItemPtr TreeItem = MakeShareable(new FAssetRuleTreeItem());
				TreeItem->RuleItem = &SingleTypeRule.Value;
				TreeItem->bIsTypeRuleItem = true;
				TreeItem->AssetType = TypeName;
				FAssetRuleItem& Rule = SingleTypeRule.Value;
				for (FInstancedStruct& AssetRuleItem : Rule.Children)
				{
					FAssetRuleTreeItemPtr ChildTreeItem = MakeShareable(new FAssetRuleTreeItem());
					ChildTreeItem->RuleItem = (FAssetRuleItem*)(AssetRuleItem.GetMemory());
					ChildTreeItem->AssetType = TypeName;
					TreeItem->Children.Add(ChildTreeItem);
					ChildTreeItem->OwnedManagerView = this;
				}
				TreeItems.Add(TreeItem);
			}
		}
	}
	if (TreeView.IsValid())
	{
		TreeView->RequestListRefresh();
	}
}

void SErdanAssetManagerView::SetupTreeViewColumns()
{
	if (!RuleTreeViewHeaderRow.IsValid())
	{
		RuleTreeViewHeaderRow = SNew(SHeaderRow)
			.Visibility(EVisibility::Visible)
			.CanSelectGeneratedColumn(true);
	}
	SHeaderRow& HeaderRow = *RuleTreeViewHeaderRow;

	auto AddColumn = [&HeaderRow,this](TSharedPtr<IErdanAssetManagerRuleTreeColumn> Column)
	{
		RuleTreeColumns.Add(Column->GetColumnID(), Column);
		auto ColumnArgs = Column->ConstructHeaderRowColumn();
		ColumnArgs.DefaultLabel(FText::FromName(Column->GetColumnID()));
		ColumnArgs.ShouldGenerateWidget(true);
		ColumnArgs.FillWidth(0.5f);
		ColumnArgs.HeaderComboVisibility(EHeaderComboVisibility::OnHover);
		ColumnArgs.HAlignCell(EHorizontalAlignment::HAlign_Left);

		HeaderRow.SetShowGeneratedColumn(Column->GetColumnID(), true);
		HeaderRow.AddColumn(ColumnArgs);
	};
	AddColumn(MakeShareable(new FErdanAssetManagerRuleTreeLabelColumn(FName("Rule Item"), *this, EHorizontalAlignment::HAlign_Left)));
	AddColumn(MakeShareable(new FErdanAssetManagerRuleTreeOperationColumn(FName("Operation"), *this, EHorizontalAlignment::HAlign_Right)));
	
}

const FString SErdanAssetManagerView::GetCurrentManagedPath() const
{
	if (ManagedPathComboBox.IsValid())
	{
		if (ManagedPathComboBox->GetSelectedItem().IsValid())
		{
			return *ManagedPathComboBox->GetSelectedItem().Get();
		}
	}
	return FString();
}

FReply SErdanAssetManagerView::OnSaveRuleData()
{
	FSaveAssetDialogConfig SaveAssetDialogConfig;
	{
		SaveAssetDialogConfig.DefaultPath = FString(PLUG_PKG_NAME);
		SaveAssetDialogConfig.AssetClassNames.Add(FTopLevelAssetPath(*UErdanAssetManagerRuleData::StaticClass()->GetPathName()));
		SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::AllowButWarn;
		SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAssetManagerRuleDataDialogTitle", "Save Rule Data As");
	}

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);
	if (!SaveObjectPath.IsEmpty())
	{
		FString PackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);

		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

		FAssetData ExistingRuleData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*SaveObjectPath), true);
		FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
		if (ExistingRuleData.IsValid())
		{

		}
		else
		{
			UPackage* Package = CreatePackage(*PackageName);
			Package->FullyLoad();
			const FString NewAssetName = FPackageName::GetLongPackageAssetName(PackageName);
			FString FilePath = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
			FSavePackageArgs SaveArgs;
			UErdanAssetManagerRuleData* NewRuleData = NewObject<UErdanAssetManagerRuleData>(Package, *NewAssetName, RF_Public | RF_Standalone);
			NewRuleData->ManagedPathsRules = RuleData->ManagedPathsRules;
			UPackage::SavePackage(Package, NewRuleData, *FilePath, SaveArgs);
		}

		UE_LOG(LogTemp, Log, TEXT("SaveObjectPath: %s"), *SaveObjectPath);
		UE_LOG(LogTemp, Log, TEXT("PackageName: %s"), *PackageName);
	}
	return FReply::Handled();
}

SErdanAssetManagerView::~SErdanAssetManagerView()
{
	if (PropertyView.IsValid())
	{
		PropertyView.Reset();
	}

	if (RuleData.IsValid())
	{
		RuleData.Reset();
	}
	
	for (TSharedPtr<FString> Str : ManagedPaths)
	{
		if (Str.IsValid())
		{
			Str.Reset();
		}
	}
	for (FAssetRuleTreeItemPtr TreeItem : TreeItems)
	{
		if (TreeItem.IsValid())
		{
			TreeItem.Reset();
		}
	}
}

TSharedPtr<FDragDropOperation> SErdanAssetManagerView::CreateDragDropOperation(const FPointerEvent& MouseEvent, const TArray<FAssetRuleTreeItemPtr>& InTreeItems) const
{
	return TSharedPtr<FDragDropOperation>();
}


#undef LOCTEXT_NAMESPACE
