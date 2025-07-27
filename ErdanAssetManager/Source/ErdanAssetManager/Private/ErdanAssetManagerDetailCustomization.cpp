 //Copyright Epic Games, Inc. All Rights Reserved.

#include "ErdanAssetManagerDetailCustomization.h"
#include "ErdanAssetManagerRuleData.h"

#include "Misc/MessageDialog.h"
#include "Misc/Guid.h"
#include "UObject/UnrealType.h"
#include "Layout/Margin.h"
#include "Misc/Attribute.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Styling/AppStyle.h"
#include "EditorSupportDelegates.h"
#include "DetailWidgetRow.h"
#include "PropertyHandle.h"
#include "IDetailPropertyRow.h"
#include "DetailLayoutBuilder.h"
#include "IDetailGroup.h"
#include "DetailCategoryBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "ScopedTransaction.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SBox.h"
#include "Modules/ModuleManager.h"
#include "AssetToolsModule.h"
#include "Curves/CurveLinearColor.h"
#include "IPropertyUtilities.h"
#include "Engine/Texture.h"
#include "HAL/PlatformApplicationMisc.h"

#define LOCTEXT_NAMESPACE "MaterialInstanceEditor"

TSharedRef<IDetailCustomization> FErdanAssetManagerDetailCustomization::MakeInstance()
{
	return MakeShareable(new FErdanAssetManagerDetailCustomization());
}

FErdanAssetManagerDetailCustomization::FErdanAssetManagerDetailCustomization()
{
}

void FErdanAssetManagerDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
    DetailLayout.HideCategory("AssetImportData");
    DetailLayout.HideCategory("ImportSettings");
    DetailLayout.HideCategory("FilePath");
    DetailLayout.HideCategory("File Path");
    DetailLayout.HideCategory("Thumbnail");

    PropertyUtilities = DetailLayout.GetPropertyUtilities();
     //获取当前正在定制的所有对象
    TArray<TWeakObjectPtr<UObject>> CustomizedObjects;
    DetailLayout.GetObjectsBeingCustomized(CustomizedObjects);    

    if (CustomizedObjects.Num() == 0)
    {
        return;
    }

    TArray<FName> CategoryNames;
    DetailLayout.GetCategoryNames(CategoryNames);
    CategoryNames.AddUnique(FName("Default"));

    UClass* TargetClass = CustomizedObjects[0]->GetClass();

    while (TargetClass)
    {
        for (TFieldIterator<FProperty> PropIt(TargetClass); PropIt; ++PropIt)
        {
            FProperty* Property = *PropIt;
            const FName PropertyName = Property->GetFName();

            TSharedPtr<IPropertyHandle> PropertyHandle = DetailLayout.GetProperty(PropertyName, TargetClass);
            const FString PropertyPath = FString(Property->GetFullName());
            if (!PropertyHandle.IsValid())
            {
                continue;
            }

            FString CategoryName = Property->GetMetaData(TEXT("Category"));
            if (CategoryName.IsEmpty())
            {
                CategoryName = "Default";
            }
            IDetailCategoryBuilder& CategoryBuilder = DetailLayout.EditCategory(FName(CategoryName));
            IDetailPropertyRow& Row = CategoryBuilder.AddProperty(PropertyHandle.ToSharedRef());
            Row.CustomWidget()
                .NameContent()
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(0.0f, 0.0f, 4.0f, 0.0f)
                        [
                            SNew(SCheckBox)
                                .Style(&FAppStyle::Get().GetWidgetStyle<FCheckBoxStyle>("Menu.CheckBox"))
                                .OnCheckStateChanged_Lambda([&,PropertyPath](ECheckBoxState NewState)
                                {
                                    TSharedPtr<FAssetRuleItemMetaData> RuleItemMetaData = DetailLayout.GetDetailsView()->GetMetaData<FAssetRuleItemMetaData>();
                                    if (RuleItemMetaData.IsValid())
                                    {
                                        FAssetRuleItem* RuleItem = RuleItemMetaData->RuleItem;
                                        if (NewState == ECheckBoxState::Checked)
                                        {
                                            RuleItem->ManagedPropertyPath.Add(PropertyPath);
                                        }
                                        else if (NewState == ECheckBoxState::Unchecked)
                                        {
                                            RuleItem->ManagedPropertyPath.Remove(PropertyPath);
                                        }
                                    }
                                })
                                .IsChecked_Lambda([&,PropertyPath]()
                                { 
                                    bool bIsChecked = false;
                                    TSharedPtr<FAssetRuleItemMetaData> RuleItemMetaData = DetailLayout.GetDetailsView()->GetMetaData<FAssetRuleItemMetaData>();
                                    if (RuleItemMetaData.IsValid())
                                    {
                                        FAssetRuleItem* RuleItem = RuleItemMetaData->RuleItem;
                                        RuleItem->ManagedPropertyPath.Contains(PropertyPath) ? bIsChecked = true : bIsChecked = false;
                                    }
                                    return bIsChecked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
                                })
                        ]
                        +SHorizontalBox::Slot()
                        .AutoWidth()
                        .Padding(0.0f, 0.0f, 4.0f, 0.0f)
                        [
                            PropertyHandle->CreatePropertyNameWidget()
                        ]
                ]
                .ValueContent()
                [
                    PropertyHandle->CreatePropertyValueWidgetWithCustomization(DetailLayout.GetDetailsView())
                ];
        }
        TargetClass = TargetClass->GetSuperClass();
    }
}


#undef LOCTEXT_NAMESPACE

