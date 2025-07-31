// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Types/ISlateMetaData.h"
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
};

struct FAssetRuleItemMetaData : public ISlateMetaData, public TSharedFromThis<FAssetRuleItemMetaData>
{
	SLATE_METADATA_TYPE(FAssetRuleItemMetaData, ISlateMetaData);
	FAssetRuleItem* RuleItem = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FAssetRuleList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FAssetRuleItem> Rules;
};


USTRUCT(BlueprintType, Blueprintable)
struct FSinglePathRules
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UClass*, FAssetRuleList> TypeRules;
};

USTRUCT(BlueprintType)
struct FDirPath 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Path)  //RelativeToGameContentDir
	FString Path = FString(TEXT("DefaultInvalidPath"));
	
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (GetOptions = GetSupportedPlatforms))
	FString TargetPlatform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (LongPackageName))
	TMap<FDirPath, FSinglePathRules> ManagedPathsRules;

public:
	UFUNCTION()
	TArray<FString> GetSupportedPlatforms();
};
