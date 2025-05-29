// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "SErdanAssetManagerRuleTreeView.h"
#include "ErdanAssetManagerRuleTreeColumn.h"

class SErdanAssetManagerView : public SCompoundWidget
{
	friend struct FAssetRuleTreeItem;
public:
	SLATE_BEGIN_ARGS(SErdanAssetManagerView) {}
	SLATE_END_ARGS()

	// Constructs this widget with InArgs
	void Construct(const FArguments& InArgs);

	virtual ~SErdanAssetManagerView();

	TSharedPtr<FDragDropOperation> CreateDragDropOperation(const FPointerEvent& MouseEvent, const TArray<FAssetRuleTreeItemPtr>& InTreeItems) const;


	const TMap<FName, TSharedPtr<IErdanAssetManagerRuleTreeColumn>>& GetRuleTreeColumns() const
	{
		return RuleTreeColumns;
	}

protected:
	TWeakObjectPtr<class UErdanAssetManagerRuleData> RuleData;

private:
	TMap<FName, TSharedPtr<IErdanAssetManagerRuleTreeColumn>> RuleTreeColumns;
	TSharedPtr<SComboBox<TSharedPtr<FString>>>  ManagedPathComboBox;
	TSharedPtr<class IDetailsView> PropertyView;
	TSharedPtr<class SButton> AddManagedPath_BT;

	TSharedPtr<SHeaderRow> RuleTreeViewHeaderRow;
	TSharedPtr<SErdanAssetManagerRuleTreeView> TreeView;
	TSharedPtr<IMenu> AddManagedPathPickerMenu;
	TArray<TSharedPtr<FString>> ManagedPaths;
	const FString GetCurrentManagedPath() const;
	TArray<FAssetRuleTreeItemPtr> TreeItems;
	FReply OnSaveRuleData();

	TSharedRef<SWidget> OnGenerateManagedPathComboItem(TSharedPtr<FString> InItem);
	void OnCurrentManagedPathChanged(TSharedPtr<FString> NewChoice, ESelectInfo::Type SelectType);
	FReply OnAddManagedPath();
	FReply OnDeleteManagedPath();

	void CreatePropertyView();
	void CreateManagedPathComboBox();
	void CreateTypeRulesTreeView();

	void InitRuleData();
	void ForceRefreshManagedPath();
	void ForceRefreshTypeRules();
	void SetupTreeViewColumns();

};
