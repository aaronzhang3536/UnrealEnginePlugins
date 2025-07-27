// Copyright Epic Games, Inc. All Rights Reserved.

#include "ErdanAssetRuleListStructCustomization.h"

#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "DetailWidgetRow.h"


class SWidget;

#define LOCTEXT_NAMESPACE "DirectoryPathStructCustomization"

TSharedRef<IPropertyTypeCustomization> FErdanAssetRuleListStructCustomization::MakeInstance()
{
	return MakeShareable(new FErdanAssetRuleListStructCustomization());
}

void FErdanAssetRuleListStructCustomization::CustomizeHeader( TSharedRef<IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
	TSharedPtr<IPropertyHandle> RulesProperty = StructPropertyHandle->GetChildHandle("Rules");
	if (!RulesProperty.IsValid()) return;
	if(RulesProperty.IsValid())
	{
		HeaderRow.ValueContent()
		.MinDesiredWidth(125.0f)
		.MaxDesiredWidth(600.0f)
		[
			RulesProperty->CreatePropertyValueWidget()
		]
		.NameContent()
		[
			StructPropertyHandle->CreatePropertyNameWidget()
		];
	}
}

void FErdanAssetRuleListStructCustomization::CustomizeChildren( TSharedRef<IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
	TSharedPtr<IPropertyHandle> RulesProperty = StructPropertyHandle->GetChildHandle("Rules");
	if (!RulesProperty.IsValid()) return;

	TSharedPtr<IPropertyUtilities> PropertyUtils = StructCustomizationUtils.GetPropertyUtilities();

	FSimpleDelegate RefreshDelegate = 
		FSimpleDelegate::CreateLambda([PropertyUtils]()
		{
			PropertyUtils->ForceRefresh();
		});

	RulesProperty->AsArray()->SetOnNumElementsChanged(RefreshDelegate);

	uint32 RulesCount = 0;
	RulesProperty->GetNumChildren(RulesCount);
	for (uint32 i = 0; i < RulesCount; ++i)
	{
		TSharedPtr<IPropertyHandle> RuleHandle = RulesProperty->GetChildHandle(i);
		if (RuleHandle.IsValid())
		{
			StructBuilder.AddProperty(RuleHandle.ToSharedRef());
		}
	}
}

#undef LOCTEXT_NAMESPACE
