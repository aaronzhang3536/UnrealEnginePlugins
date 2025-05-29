// Copyright Epic Games, Inc. All Rights Reserved.

#include "ErdanAssetManagerCommands.h"

#define LOCTEXT_NAMESPACE "FErdanAssetManagerModule"

void FErdanAssetManagerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "ErdanAssetManager", "Bring up ErdanAssetManager window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
