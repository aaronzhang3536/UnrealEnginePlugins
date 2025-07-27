#include "ErdanAssetManagerRuleDataCustomization.h"

#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "ErdanAssetManagerRuleData.h"


#define LOCTEXT_NAMESPACE "ErdanAssetManagerRuleDataCustomization"

TSharedRef<IDetailCustomization> FErdanAssetManagerRuleDataCustomization::MakeInstance()
{
	return MakeShareable(new FErdanAssetManagerRuleDataCustomization);
}

void FErdanAssetManagerRuleDataCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.HideProperty("General");
	TSharedPtr<IPropertyHandle> MapProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UErdanAssetManagerRuleData, ManagedPathsRules));

	if (!MapProperty.IsValid() || !MapProperty->IsValidHandle()) return;
	MapProperty->MarkHiddenByCustomization();

	{
        // 在单独的一行中显示Map的增删操作按钮
		IDetailCategoryBuilder& EmptyCategory = DetailBuilder.EditCategory("aaa ", FText::GetEmpty(), ECategoryPriority::Important);
		EmptyCategory.SetIsEmpty(true);
		// Add the action buttons
		EmptyCategory.HeaderContent(MapProperty->CreateDefaultPropertyButtonWidgets(), true);
		EmptyCategory.AddCustomRow(FText::GetEmpty());	
		EmptyCategory.SetCategoryVisibility(true);
	}

	TSharedPtr<IPropertyHandleMap> MapHandle = MapProperty->AsMap();
	if (!MapHandle.IsValid()) return;

	uint32 NumElements = 0;
	MapHandle->GetNumElements(NumElements);

	for (uint32 Index = 0; Index < NumElements; ++Index)
	{
		TSharedPtr<IPropertyHandle> EntryHandle = MapProperty->GetChildHandle(Index);
		if (!EntryHandle.IsValid()) continue;

		TSharedPtr<IPropertyHandle> KeyHandle = EntryHandle->GetKeyHandle();

		void* ValuePtr = nullptr;
		FSinglePathRules SinglePathRules;
		FPropertyAccess::Result Result = EntryHandle->GetValueData(ValuePtr);

		if (Result == FPropertyAccess::Success)
		{
			SinglePathRules = *reinterpret_cast<FSinglePathRules*>(ValuePtr);

			if (!KeyHandle.IsValid()) continue;


			FString PathStr;
			KeyHandle->GetChildHandle("Path")->GetValue(PathStr);
			if (PathStr.TrimStartAndEnd().IsEmpty()) continue;

			TSharedPtr<IPropertyHandle> TypeRulesHandle = EntryHandle->GetChildHandle("TypeRules");

			IDetailCategoryBuilder& Category = DetailBuilder.EditCategory(*PathStr);
			Category.HeaderContent(
				SNew(SHorizontalBox)
					+  SHorizontalBox::Slot()
					[
						KeyHandle->CreatePropertyValueWidgetWithCustomization(DetailBuilder.GetDetailsView())
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					[
						KeyHandle->CreateDefaultPropertyButtonWidgets()
					], true);
			IDetailPropertyRow& Row = Category.AddProperty(TypeRulesHandle.ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE