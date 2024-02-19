// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WorldInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FFQFWorldInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 WorldIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WorldName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>  AbilityGrantedOnCompletion;
	


};
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UWorldInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFQFWorldInfo> WorldInformation;

};
