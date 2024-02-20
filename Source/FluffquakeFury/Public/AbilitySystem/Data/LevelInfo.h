// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "LevelInfo.generated.h"

USTRUCT(BlueprintType)
struct FFQFLevelInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 LevelIndexInWorld = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 WorldIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 LevelLength = 60;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MinimumSurvivalLength = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>  AbilityGrantedOnCompletion;
	


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

	int32 CountLevelsInWorld(int32 WorldIndex);

};
