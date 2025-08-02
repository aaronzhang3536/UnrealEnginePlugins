// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/UnrealString.h"
#include "IPropertyTypeCustomization.h"
#include "Input/Reply.h"
#include "Internationalization/Text.h"
#include "Templates/SharedPointer.h"

class IMenu;
class IPropertyHandle;
class SButton;

class FAssetRuleItemStructCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader( TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils ) override;

	virtual void CustomizeChildren( TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils ) override;              

private:

	void CustomizeRuleObjectProperty(TSharedRef<IPropertyHandle> StructPropertyHandle);
	void CustomizeRegularExpressionProperty(TSharedRef<IPropertyHandle> StructPropertyHandle);
	void CustomizeManagedPropertyPathProperty(TSharedRef<IPropertyHandle> StructPropertyHandle);

	//FReply OnPickDirectory(TSharedRef<IPropertyHandle> PropertyHandle, const bool bRelativeToGameContentDir, const bool bUseRelativePaths) const;

	//void OnPathPicked(const FString& Path, TSharedRef<IPropertyHandle> PropertyHandle);

private:

	/** The pick button widget */
	TSharedPtr<SButton> Button_OpenRegexEditor;

};
