// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/InstancedStruct.h"

#include "ErdanAssetManagerRuleData.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FAssetRuleItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString RegularExpression = FString();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* RuleObject = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> ManagedPropertyPath;

	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/Script/ErdanAssetManager.AssetRuleItem"))
	TArray<FInstancedStruct> Children;
};


USTRUCT(BlueprintType, Blueprintable)
struct FSinglePathRules
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UClass*, FAssetRuleItem> TypeRules;
};

/*
-Path
--Type1
---Rule1
---Rule2
---Rule3
--Type2
---Rule1
---Rule2
---Rule3
*/

//////////////////////////////////////////////////////////////////////////
// SErdanAssetManagerView
UCLASS(BlueprintType, Blueprintable)
class UErdanAssetManagerRuleData : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<FString, FSinglePathRules> ManagedPathsRules;
};
