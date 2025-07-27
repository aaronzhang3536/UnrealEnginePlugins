#include "ErdanAssetManagerRuleTreeColumn.h"

#define LOCTEXT_NAMESPACE "ErdanAssetManagerRuleTreeColumn"

IErdanAssetManagerRuleTreeColumn::IErdanAssetManagerRuleTreeColumn(SErdanAssetManagerView& AssetManagerView, EHorizontalAlignment InHAlignCell)
	:WeakAssetManagerView(StaticCastSharedRef<SErdanAssetManagerView>(AssetManagerView.AsShared()))
	,HAlignCell(InHAlignCell)
{
}



FErdanAssetManagerRuleTreeLabelColumn::FErdanAssetManagerRuleTreeLabelColumn(FName InID, SErdanAssetManagerView& AssetManagerView, EHorizontalAlignment InHAlignCell) 
	:IErdanAssetManagerRuleTreeColumn(AssetManagerView, InHAlignCell), ID(InID)
{
}

SHeaderRow::FColumn::FArguments FErdanAssetManagerRuleTreeLabelColumn::ConstructHeaderRowColumn()
{
	return SHeaderRow::Column(GetColumnID())
		.HAlignHeader(HAlign_Left)
		.VAlignHeader(VAlign_Center)
		.HAlignCell(HAlign_Left)
		.VAlignCell(VAlign_Center)
		.DefaultTooltip(FText::FromName(GetColumnID()))
		[
			SNew(STextBlock)
				.Margin(FMargin(0.0f))
				.Text(FText::FromName(ID))
		];
};

const TSharedRef<SWidget> FErdanAssetManagerRuleTreeLabelColumn::ConstructRowWidget(FAssetRuleTreeItemRef TreeItem, const TSharedRef<SAssetRuleTreeRow> Row)
{
	return
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		[
			SNew(SExpanderArrow, Row).IndentAmount(12)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		.Padding(FMargin(4.0f, 0.0f))
		.VAlign(VAlign_Center)
		[
			TreeItem->bIsTypeRuleItem ?
				SNew(SInlineEditableTextBlock).Text(FText::FromString(TreeItem->AssetType)).IsReadOnly(true)
				: SNew(SInlineEditableTextBlock).Text(FText::FromString(TreeItem->RuleItem->RegularExpression))
		];
}

SHeaderRow::FColumn::FArguments FErdanAssetManagerRuleTreeOperationColumn::ConstructHeaderRowColumn()
{
	return SHeaderRow::Column(GetColumnID())
		.HAlignHeader(HAlign_Left)
		.VAlignHeader(VAlign_Center)
		.HAlignCell(HAlign_Left)
		.VAlignCell(VAlign_Center)
		.DefaultTooltip(FText::FromName(GetColumnID()))
		[
			SNew(STextBlock)
				.Margin(FMargin(0.0f))
				.Text(FText::FromName(ID))
		];
}
const TSharedRef<SWidget> FErdanAssetManagerRuleTreeOperationColumn::ConstructRowWidget(FAssetRuleTreeItemRef TreeItem, const TSharedRef<SAssetRuleTreeRow> Row)
{
	if (TreeItem->bIsTypeRuleItem)
	{
		return
			SNew(SBox)
			.HAlign(HAlign_Right)
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(2.0, 2.0, 1.0, 2.0)
					[
						SNew(SButton)
							.ToolTipText(LOCTEXT("CopyRegularRule", "Copy"))
							.ContentPadding(2.0f)
							.ForegroundColor(FSlateColor::UseForeground())
							.IsFocusable(false)
							.Text(FText::FromString("Copy"))
							.OnClicked_Lambda([TreeItem]()
							{
								FString RegularExpression = TreeItem->RuleItem->RegularExpression;
								return FReply::Handled();
							})
							[
								SNew(SImage)
									.Image(FAppStyle::Get().GetBrush("GenericCommands.Copy"))
							]
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(1.0, 2.0, 1.0, 2.0)
					[
						SNew(SButton)
							[
								SNew(SImage)
									.Image(FAppStyle::Get().GetBrush("GenericCommands.Paste"))								
							]
							//.Text(FText::FromString("Paste"))
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(1.0, 2.0, 1.0, 2.0)
					[
						SNew(SButton)
							[
								SNew(SImage)
									.Image(FAppStyle::Get().GetBrush("Icons.Plus"))
									.ColorAndOpacity(FStyleColors::AccentGreen)
							]
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(1.0, 2.0, 2.0, 2.0)
					[
						SNew(SButton)
							[
								SNew(SImage)
									.Image(FAppStyle::Get().GetBrush("Icons.Minus"))
									.ColorAndOpacity(FStyleColors::AccentRed)
							]
					]
			];
	}
	else
	{
		return SNew(SBox)
			.HAlign(HAlign_Right)
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(2.0, 2.0, 1.0, 2.0)
					[
						SNew(SButton)
							.Text(FText::FromString("Delete"))
					]
					+ SHorizontalBox::Slot()
					.VAlign(VAlign_Center)
					.AutoWidth()
					.Padding(1.0, 2.0, 2.0, 2.0)
					[
						SNew(SButton)
							.Text(FText::FromString("Rename"))
					]
			];
	}
	return SNullWidget::NullWidget;
}


#undef LOCTEXT_NAMESPACE