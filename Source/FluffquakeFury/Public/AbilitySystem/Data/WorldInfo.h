// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "Engine/DataAsset.h"
#include "WorldInfo.generated.h"

// USTRUCT(BlueprintType)
// struct FFQFWorldInfo
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
// 	int32 WorldIndex;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
// 	FName WorldName = FName();
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
// 	TSubclassOf<UGameplayAbility>  AbilityGrantedOnCompletion;
// 	
//
//
// };
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UWorldInfo : public UDataAsset
{
	GENERATED_BODY()
	
// public:
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	TArray<FFQFWorldInfo> WorldInformation;

};
