// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

struct FGameplayTag;

USTRUCT(BlueprintType)
struct FFQFLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName LevelName = "";

	UPROPERTY(EditDefaultsOnly)
	int32 LevelLength = 60;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MinimumSurvivalLength = 30;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilityGrantedOnCompletionTag = FGameplayTag();


};
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API ULevelInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFQFLevelInfo> LevelInformation;
	
};
