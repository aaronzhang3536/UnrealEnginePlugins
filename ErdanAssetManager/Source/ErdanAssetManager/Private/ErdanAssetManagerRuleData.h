// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"

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

USTRUCT(BlueprintType)
struct FDirPath 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)
	FString Path;
	
	bool operator==(const FDirPath& Other) const
	{
		return Path == Other.Path;
	}

	friend uint32 GetTypeHash(const FDirPath& Key)
	{
		return GetTypeHash(Key.Path);
	}
	 bool IsEmpty() const
	{
		return Path.IsEmpty();
	}
};

//////////////////////////////////////////////////////////////////////////
// SErdanAssetManagerView
UCLASS(BlueprintType, Blueprintable)
class UErdanAssetManagerRuleData : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FDirPath, FSinglePathRules> ManagedPathsRules;
};
