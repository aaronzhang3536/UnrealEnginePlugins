// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ErdanAssetManagerStyle.h"

class FErdanAssetManagerCommands : public TCommands<FErdanAssetManagerCommands>
{
public:

	FErdanAssetManagerCommands()
		: TCommands<FErdanAssetManagerCommands>(TEXT("ErdanAssetManager"), NSLOCTEXT("Contexts", "ErdanAssetManager", "ErdanAssetManager Plugin"), NAME_None, FErdanAssetManagerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};