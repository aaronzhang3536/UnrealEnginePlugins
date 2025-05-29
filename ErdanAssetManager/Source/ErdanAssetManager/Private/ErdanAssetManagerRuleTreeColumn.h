#pragma once
#include "SErdanAssetManagerView.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

class IErdanAssetManagerRuleTreeColumn : public TSharedFromThis<IErdanAssetManagerRuleTreeColumn>
{
public:
	IErdanAssetManagerRuleTreeColumn(SErdanAssetManagerView& AssetManagerView, EHorizontalAlignment InHAlignCell = EHorizontalAlignment::HAlign_Center);
	virtual ~IErdanAssetManagerRuleTreeColumn() = default;
	virtual FName GetColumnID() = 0;;
	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() = 0;
	virtual const TSharedRef<SWidget> ConstructRowWidget(FAssetRuleTreeItemRef TreeItem, const TSharedRef<SAssetRuleTreeRow> Row) = 0;
	EHorizontalAlignment HAlignCell;
private:
	TWeakPtr<SErdanAssetManagerView> WeakAssetManagerView;
};

class FErdanAssetManagerRuleTreeLabelColumn : public IErdanAssetManagerRuleTreeColumn
{
public:
	FErdanAssetManagerRuleTreeLabelColumn(FName InID, SErdanAssetManagerView& AssetManagerView, EHorizontalAlignment InHAlignCell = EHorizontalAlignment::HAlign_Center);
	~FErdanAssetManagerRuleTreeLabelColumn() {};
	virtual FName GetColumnID() override { return ID; }
	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;
	virtual const TSharedRef<SWidget> ConstructRowWidget(FAssetRuleTreeItemRef TreeItem, const TSharedRef<SAssetRuleTreeRow> Row) override;
private:
	TWeakPtr<SErdanAssetManagerView> WeakAssetManagerView;
	FName ID;
};

class FErdanAssetManagerRuleTreeOperationColumn : public IErdanAssetManagerRuleTreeColumn
{
public:
	FErdanAssetManagerRuleTreeOperationColumn(FName InID, SErdanAssetManagerView& AssetManagerView, EHorizontalAlignment InHAlignCell = EHorizontalAlignment::HAlign_Center) :IErdanAssetManagerRuleTreeColumn(AssetManagerView,InHAlignCell), ID(InID) {};
	~FErdanAssetManagerRuleTreeOperationColumn() {};
	virtual FName GetColumnID() override { return ID; }
	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;
	virtual const TSharedRef<SWidget> ConstructRowWidget(FAssetRuleTreeItemRef TreeItem, const TSharedRef<SAssetRuleTreeRow> Row) override;
private:
	TWeakPtr<SErdanAssetManagerView> WeakAssetManagerView;
	FName ID;
};
