// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "WorldInfoNew.generated.h"

USTRUCT(BlueprintType)
struct FFQFWorldInfoNew
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 WorldIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName WorldName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>  AbilityGrantedOnCompletion;
	


};
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UWorldInfoNew : public UDataAsset
{
	GENERATED_BODY()

		
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFQFWorldInfoNew> WorldInformation;

};
