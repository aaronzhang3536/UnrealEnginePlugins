// Copyright Epic Games, Inc. All Rights Reserved.

#include "ErdanAssetManagerStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FErdanAssetManagerStyle::StyleInstance = nullptr;

void FErdanAssetManagerStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FErdanAssetManagerStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FErdanAssetManagerStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ErdanAssetManagerStyle"));
	return StyleSetName;
}

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FErdanAssetManagerStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("ErdanAssetManagerStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("ErdanAssetManager")->GetBaseDir() / TEXT("Resources"));

	Style->Set("ErdanAssetManager.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));

	return Style;
}

void FErdanAssetManagerStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FErdanAssetManagerStyle::Get()
{
	return *StyleInstance;
}
