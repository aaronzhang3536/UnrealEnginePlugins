// Copyright Epic Games, Inc. All Rights Reserved.

#include "SErdanAssetManagerRuleTreeView.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/AppStyle.h"
#include "DragAndDrop/DecoratedDragDropOp.h"
#include "Styling/StyleColors.h"

#include "SErdanAssetManagerView.h"
#include "ErdanAssetManagerRuleTreeColumn.h"

#define LOCTEXT_NAMESPACE "SSceneOutliner"

//static void UpdateOperationDecorator(const FDragDropEvent& Event, const FSceneOutlinerDragValidationInfo& ValidationInfo)
//{
//	const FSlateBrush* Icon = ValidationInfo.IsValid() ? FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.OK")) : FAppStyle::GetBrush(TEXT("Graph.ConnectorFeedback.Error"));
//
//	FDragDropOperation* Operation = Event.GetOperation().Get();
//	if (Operation && Operation->IsOfType<FDecoratedDragDropOp>())
//	{
//		auto* DecoratedOp = static_cast<FDecoratedDragDropOp*>(Operation);
//		DecoratedOp->SetToolTip(ValidationInfo.ValidationText, Icon);
//	}
//}

//static void ResetOperationDecorator(const FDragDropEvent& Event)
//{
//	FDragDropOperation* Operation = Event.GetOperation().Get();
//	if (Operation)
//	{
//		if (Operation->IsOfType<FSceneOutlinerDragDropOp>())
//		{
//			static_cast<FSceneOutlinerDragDropOp*>(Operation)->ResetTooltip();
//		}
//		else if (Operation->IsOfType<FDecoratedDragDropOp>())
//		{
//			static_cast<FDecoratedDragDropOp*>(Operation)->ResetToDefaultToolTip();
//		}
//	}
//}

static FReply HandleOnDragDetected( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, TWeakPtr<SErdanAssetManagerRuleTreeView> Table )
{
	auto TablePtr = Table.Pin();
	if (TablePtr.IsValid() && MouseEvent.IsMouseButtonDown( EKeys::LeftMouseButton ))
	{
		auto Operation = TablePtr->GetAssetManagerViewPtr().Pin()->CreateDragDropOperation(MouseEvent, TablePtr->GetSelectedItems());

		if (Operation.IsValid())
		{
			return FReply::Handled().BeginDragDrop(Operation.ToSharedRef());
		}
	}

	return FReply::Unhandled();
}

//FReply HandleDrop(TSharedPtr<SSceneOutliner> SceneOutlinerPtr, const FDragDropEvent& DragDropEvent, ISceneOutlinerTreeItem& DropTarget, FSceneOutlinerDragValidationInfo& ValidationInfo, bool bApplyDrop = false)
//{
//	if (!SceneOutlinerPtr.IsValid())
//	{
//		return FReply::Unhandled();
//	}
//
//	// Don't handle this if we're not showing a hierarchy
//	const FSharedSceneOutlinerData& SharedData = SceneOutlinerPtr->GetSharedData();
//	if (!SharedData.bShowParentTree)
//	{
//		return FReply::Unhandled();
//	}
//
//	// Don't handle this if the scene outliner is not in a mode which supports drag and drop
//	if (!SceneOutlinerPtr->CanSupportDragAndDrop())
//	{
//		return FReply::Unhandled();
//	}
//
//	FSceneOutlinerDragDropPayload DraggedObjects(*DragDropEvent.GetOperation());
//	// Validate now to make sure we don't doing anything we shouldn't
//	if (!SceneOutlinerPtr->ParseDragDrop(DraggedObjects, *DragDropEvent.GetOperation()))
//	{
//		return FReply::Unhandled();
//	}
//
//	ValidationInfo = SceneOutlinerPtr->ValidateDrop(StaticCast<ISceneOutlinerTreeItem&>(DropTarget), DraggedObjects);
//
//	if (!ValidationInfo.IsValid())
//	{
//		// Return handled here to stop anything else trying to handle it - the operation is invalid as far as we're concerned
//		return FReply::Handled();
//	}
//
//	if (bApplyDrop)
//	{
//		SceneOutlinerPtr->OnDropPayload(DropTarget, DraggedObjects, ValidationInfo);
//	}
//
//	return FReply::Handled();
//}

//FReply HandleDropFromWeak(TWeakPtr<SSceneOutliner> SceneOutlinerWeak, const FDragDropEvent& DragDropEvent, FSceneOutlinerDragValidationInfo& ValidationInfo, bool bApplyDrop = false)
//{
//	const ISceneOutlinerMode* Mode = SceneOutlinerWeak.IsValid() ? SceneOutlinerWeak.Pin()->GetMode() : nullptr;
//	FFolder::FRootObject RootObject = Mode ? Mode->GetRootObject() : FFolder::GetInvalidRootObject();
//	FFolder RootFolder(RootObject);
//	FFolderTreeItem DropTarget(RootFolder);
//	return HandleDrop(SceneOutlinerWeak.Pin(), DragDropEvent, DropTarget, ValidationInfo, bApplyDrop);
//}

void SErdanAssetManagerRuleTreeView::Construct(const SErdanAssetManagerRuleTreeView::FArguments& InArgs, TSharedRef<SErdanAssetManagerView> Owner)
{
	SceneOutlinerWeak = Owner;
	STreeView::Construct(InArgs);
}

//void SErdanAssetManagerRuleTreeView::FlashHighlightOnItem(FAssetRuleTreeItemPtr FlashHighlightOnItem )
//{
//	TSharedPtr< SAssetRuleTreeRow > RowWidget = StaticCastSharedPtr< SAssetRuleTreeRow >( WidgetGenerator.GetWidgetForItem( FlashHighlightOnItem ) );
//	if( RowWidget.IsValid() )
//	{
//		RowWidget->FlashHighlight();
//	}
//}

FReply SErdanAssetManagerRuleTreeView::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Log, TEXT("OnDragOver"))
	//FSceneOutlinerDragValidationInfo ValidationInfo = FSceneOutlinerDragValidationInfo::Invalid();
	//auto Reply = HandleDropFromWeak(SceneOutlinerWeak, DragDropEvent, ValidationInfo);
	//if (Reply.IsEventHandled())
	//{
	//	UpdateOperationDecorator(DragDropEvent, ValidationInfo);
	//}

	//return Reply;
	return FReply::Handled();
}

void SErdanAssetManagerRuleTreeView::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Log, TEXT("OnDragLeave"));
	//if (!SceneOutlinerWeak.IsValid())
	//{
	//	return;
	//}

	//if( SceneOutlinerWeak.Pin()->GetSharedData().bShowParentTree )
	//{
	//	ResetOperationDecorator(DragDropEvent);
	//}
}

FReply SErdanAssetManagerRuleTreeView::OnDrop(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Log, TEXT("OnDrop"));
	//FSceneOutlinerDragValidationInfo ValidationInfo = FSceneOutlinerDragValidationInfo::Invalid();
	//return HandleDropFromWeak(SceneOutlinerWeak, DragDropEvent, ValidationInfo, true);
	return FReply::Handled();
}

//void SErdanAssetManagerRuleTreeView::Private_UpdateParentHighlights()
//{
//	this->ClearHighlightedItems();
//
//	// For the Outliner, we want to highlight parent items even if the current selection is not visible (i.e collapsed)
//	for( typename TItemSet::TConstIterator SelectedItemIt(SelectedItems); SelectedItemIt; ++SelectedItemIt )
//	{
//		auto Parent = (*SelectedItemIt)->GetParent();
//		while (Parent.IsValid())
//		{
//			Private_SetItemHighlighted(Parent, true);
//			Parent = Parent->GetParent();
//		}
//	}
//}

FReply SAssetRuleTreeRow::OnDrop( const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent )
{
	UE_LOG(LogTemp, Warning, TEXT("SAssetRuleTreeRow::OnDrop"));
//	auto ItemPtr = Item.Pin();
//	auto SceneOutlinerPtr = SceneOutlinerWeak.Pin();
//	if (ItemPtr.IsValid() && SceneOutlinerPtr.IsValid())
//	{
//		FSceneOutlinerDragValidationInfo ValidationInfo = FSceneOutlinerDragValidationInfo::Invalid();
//		return HandleDrop(SceneOutlinerPtr, DragDropEvent, *ItemPtr, ValidationInfo, true);
//	}
//
	return FReply::Unhandled();
}

void SAssetRuleTreeRow::OnDragEnter( const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent )
{
	UE_LOG(LogTemp, Warning, TEXT("SAssetRuleTreeRow::OnDragEnter"));
//	auto ItemPtr = Item.Pin();
//	auto SceneOutlinerPtr = SceneOutlinerWeak.Pin();
//	if (ItemPtr.IsValid() && SceneOutlinerPtr.IsValid())
//	{
//		FSceneOutlinerDragValidationInfo ValidationInfo = FSceneOutlinerDragValidationInfo::Invalid();
//
//		FReply Reply = HandleDrop(SceneOutlinerPtr, DragDropEvent, *ItemPtr, ValidationInfo, false);
//		if (Reply.IsEventHandled())
//		{
//			UpdateOperationDecorator(DragDropEvent, ValidationInfo);
//		}
//	}
}

void SAssetRuleTreeRow::OnDragLeave( const FDragDropEvent& DragDropEvent )
{
	UE_LOG(LogTemp, Warning, TEXT("SAssetRuleTreeRow::OnDragLeave"));
//	auto ItemPtr = Item.Pin();
//	auto SceneOutlinerPtr = SceneOutlinerWeak.Pin();
//
//	ResetOperationDecorator(DragDropEvent);
}

FReply SAssetRuleTreeRow::OnDragOver(const FGeometry& MyGeometry, const FDragDropEvent& DragDropEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("SAssetRuleTreeRow::OnDragOver"));
//	auto SceneOutlinerPtr = SceneOutlinerWeak.Pin();
//	if (SSceneOutliner* SceneOutliner = SceneOutlinerPtr.Get())
//	{
//		if (const auto* ItemPtr = Item.Pin().Get())
//		{
//			return SceneOutliner->OnDragOverItem(DragDropEvent, *ItemPtr);
//		}
//		return FReply::Unhandled();
//	}
//
	return FReply::Handled();
}

FReply SAssetRuleTreeRow::OnMouseButtonDown( const FGeometry& MyGeometry, const FPointerEvent& MouseEvent )
{
	UE_LOG(LogTemp, Warning, TEXT("SAssetRuleTreeRow::OnMouseButtonDown"));
	auto ItemPtr = Item.Pin();
	if (ItemPtr.IsValid() && ItemPtr->CanInteract())
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			FReply Reply = SMultiColumnTableRow<FAssetRuleTreeItemPtr>::OnMouseButtonDown( MyGeometry, MouseEvent );

			//if (AssetManagerViewWeak.Pin()->CanSupportDragAndDrop())
			{
				return Reply.DetectDrag( SharedThis(this) , EKeys::LeftMouseButton );
			}

			return Reply.PreventThrottling();
		}
	}

	return FReply::Handled();
}

FReply SAssetRuleTreeRow::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("SAssetRuleTreeRow::OnMouseButtonUp"));
	auto ItemPtr = Item.Pin();
	// We don't to change the selection when it is a left click since this was handle in the on mouse down
	if (ItemPtr.IsValid() && ItemPtr->CanInteract())
	{
		return SMultiColumnTableRow<FAssetRuleTreeItemPtr>::OnMouseButtonUp(MyGeometry, MouseEvent);
	}

	return FReply::Handled();
}

FReply SAssetRuleTreeRow::OnMouseButtonDoubleClick(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("SAssetRuleTreeRow::OnMouseButtonDoubleClick"));
	auto ItemPtr = Item.Pin();
	// We don't want to act on double click on an item that can't be interacted with
	if (ItemPtr.IsValid() && ItemPtr->CanInteract())
	{
		return SMultiColumnTableRow<FAssetRuleTreeItemPtr>::OnMouseButtonDoubleClick(MyGeometry, MouseEvent);
	}

	return FReply::Handled();
}

TSharedRef<SWidget> SAssetRuleTreeRow::GenerateWidgetForColumn( const FName& ColumnName )
{
	auto ItemPtr = Item.Pin();
	if (!ItemPtr.IsValid())
	{
		return SNullWidget::NullWidget;
	}


	auto Outliner = AssetManagerViewWeak.Pin();
	check(Outliner.IsValid());

	// Create the widget for this item
	TSharedRef<SWidget> NewItemWidget = SNullWidget::NullWidget;
	TSharedPtr<IErdanAssetManagerRuleTreeColumn> Column = Outliner->GetRuleTreeColumns().FindRef(ColumnName);
	if (Column.IsValid())
	{
		NewItemWidget = Column->ConstructRowWidget(ItemPtr.ToSharedRef(), SharedThis(this));
	}

	return NewItemWidget;
}

void SAssetRuleTreeRow::Construct( const FArguments& InArgs, const TSharedRef<SErdanAssetManagerRuleTreeView>& AssetManagerRuleTreeView, TSharedRef<SErdanAssetManagerView> AssetManagerView)
{
	Item = InArgs._Item->AsShared();
	AssetManagerViewWeak = AssetManagerView;
	LastHighlightInteractionTime = 0.0;

	auto Args = FSuperRowType::FArguments()
		.Style(&FAppStyle::Get().GetWidgetStyle<FTableRowStyle>("SceneOutliner.TableViewRow"))
		.ExpanderStyleSet(&FCoreStyle::Get());


	Args.OnDragDetected_Static(HandleOnDragDetected, TWeakPtr<SErdanAssetManagerRuleTreeView>(AssetManagerRuleTreeView));

	SMultiColumnTableRow<FAssetRuleTreeItemPtr>::Construct(Args, AssetManagerRuleTreeView);
}

const float SAssetRuleTreeRow::HighlightRectLeftOffset = 0.0f;
const float SAssetRuleTreeRow::HighlightRectRightOffset = 0.0f;
const float SAssetRuleTreeRow::HighlightTargetSpringConstant = 25.0f;
const float SAssetRuleTreeRow::HighlightTargetEffectDuration = 0.5f;
const float SAssetRuleTreeRow::HighlightTargetOpacity = 0.8f;
const float SAssetRuleTreeRow::LabelChangedAnimOffsetPercent = 0.2f;

//void SAssetRuleTreeRow::FlashHighlight()
//{
//    LastHighlightInteractionTime = FSlateApplication::Get().GetCurrentTime();
//}

void SAssetRuleTreeRow::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	// Call parent implementation.
	SCompoundWidget::Tick( AllottedGeometry, InCurrentTime, InDeltaTime );

	// We'll draw with the 'focused' look if we're either focused or we have a context menu summoned
	const bool bShouldAppearFocused = HasKeyboardFocus();

	// Update highlight 'target' effect
	{
		const float HighlightLeftX = HighlightRectLeftOffset;
		const float HighlightRightX = HighlightRectRightOffset + AllottedGeometry.GetLocalSize().X;

		HighlightTargetLeftSpring.SetTarget( HighlightLeftX );
		HighlightTargetRightSpring.SetTarget( HighlightRightX );

		float TimeSinceHighlightInteraction = (float)( InCurrentTime - LastHighlightInteractionTime );
		if( TimeSinceHighlightInteraction <= HighlightTargetEffectDuration || bShouldAppearFocused )
		{
			HighlightTargetLeftSpring.Tick( InDeltaTime );
			HighlightTargetRightSpring.Tick( InDeltaTime );
		}
	}
}

int32 SAssetRuleTreeRow::OnPaint( const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled ) const
{
	int32 StartLayer = SMultiColumnTableRow::OnPaint( Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled );

	const int32 TextLayer = 1;

	// See if a disabled effect should be used
	bool bEnabled = ShouldBeEnabled( bParentEnabled );
	ESlateDrawEffect DrawEffects = (bEnabled) ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

	const double CurrentTime = FSlateApplication::Get().GetCurrentTime();

	// We'll draw with the 'focused' look if we're either focused or we have a context menu summoned
	const bool bShouldAppearFocused = HasKeyboardFocus();

	// Draw highlight targeting effect
	const float TimeSinceHighlightInteraction = (float)( CurrentTime - LastHighlightInteractionTime );
	if( TimeSinceHighlightInteraction <= HighlightTargetEffectDuration )
	{

		// Compute animation progress
		float EffectAlpha = FMath::Clamp( TimeSinceHighlightInteraction / HighlightTargetEffectDuration, 0.0f, 1.0f );
		EffectAlpha = 1.0f - EffectAlpha * EffectAlpha;  // Inverse square falloff (looks nicer!)

		// Apply extra opacity falloff when dehighlighting
		float EffectOpacity = EffectAlpha;

		// Figure out a universally visible highlight color.
		FLinearColor HighlightTargetColorAndOpacity = ( (FLinearColor::White - GetColorAndOpacity())*0.5f + FLinearColor(+0.4f, +0.1f, -0.2f)) * InWidgetStyle.GetColorAndOpacityTint();
		HighlightTargetColorAndOpacity.A = HighlightTargetOpacity * EffectOpacity * 255.0f;

		// Compute the bounds offset of the highlight target from where the highlight target spring
		// extents currently lie.  This is used to "grow" or "shrink" the highlight as needed.
		const float LabelChangedAnimOffset = LabelChangedAnimOffsetPercent * AllottedGeometry.GetLocalSize().Y;

		// Choose an offset amount depending on whether we're highlighting, or clearing highlight
		const float EffectOffset = EffectAlpha * LabelChangedAnimOffset;

		const float HighlightLeftX = HighlightTargetLeftSpring.GetPosition() - EffectOffset;
		const float HighlightRightX = HighlightTargetRightSpring.GetPosition() + EffectOffset;
		const float HighlightTopY = 0.0f - LabelChangedAnimOffset;
		const float HighlightBottomY = AllottedGeometry.GetLocalSize().Y + EffectOffset;

		const FVector2D DrawPosition = FVector2D( HighlightLeftX, HighlightTopY );
		const FVector2D DrawSize = FVector2D( HighlightRightX - HighlightLeftX, HighlightBottomY - HighlightTopY );

		const FSlateBrush* StyleInfo = FAppStyle::GetBrush("SceneOutliner.ChangedItemHighlight");

		// NOTE: We rely on scissor clipping for the highlight rectangle
		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + TextLayer,
			AllottedGeometry.ToPaintGeometry( DrawSize, FSlateLayoutTransform(DrawPosition) ),	// Position, Size, Scale
			StyleInfo,													// Style
			DrawEffects,												// Effects to use
			HighlightTargetColorAndOpacity );							// Color
	}

	return FMath::Max(StartLayer, LayerId + TextLayer);
}

#undef LOCTEXT_NAMESPACE
