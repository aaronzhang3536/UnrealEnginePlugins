// Copyright Epic Games, Inc. All Rights Reserved.

#include "ErdanAssetManagerRuleData.h"
#include "Interfaces/ITargetPlatformManagerModule.h"
UErdanAssetManagerRuleData::UErdanAssetManagerRuleData(class FObjectInitializer const&)
{

}

#undef LOCTEXT_NAMESPACE

TArray<FString> UErdanAssetManagerRuleData::GetSupportedPlatforms()
{
	TArray<FString> OutPlatforms;
	ITargetPlatformManagerModule& PlatformManager = GetTargetPlatformManagerRef();
	
	for (const ITargetPlatform* Platform : PlatformManager.GetTargetPlatforms())
	{
		if (Platform)
		{
			OutPlatforms.Add(Platform->PlatformName());
		}
	}
	return OutPlatforms;
}