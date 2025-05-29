// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types/SlateEnums.h"
#include "Layout/Visibility.h"
#include "IDetailCustomization.h"
#include "Input/Reply.h"
#include "Customizations/ColorStructCustomization.h"


struct FAssetData;
class IDetailGroup;
class IDetailLayoutBuilder;
class IPropertyHandle;

DECLARE_DELEGATE_OneParam(FGetShowHiddenParameters, bool&);

/*-----------------------------------------------------------------------------
   FErdanAssetManagerDetailCustomization
-----------------------------------------------------------------------------*/

class FErdanAssetManagerDetailCustomization : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<class IDetailCustomization> MakeInstance();
	
	/** Constructor */
	FErdanAssetManagerDetailCustomization();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;

	/** Associated FMaterialInstance utilities */
	TWeakPtr<class IPropertyUtilities> PropertyUtilities;
};

