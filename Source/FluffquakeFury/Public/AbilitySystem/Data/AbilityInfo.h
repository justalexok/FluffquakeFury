// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "AbilityInfo.generated.h"

class UFQFGameplayAbility;

USTRUCT(BlueprintType)
struct FFQFAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketTagName = FName();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUsesWeapon = false;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// USoundBase* ActionSound = nullptr;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	// USoundBase* ImpactSound = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();	

// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	TObjectPtr<const UTexture2D> Icon = nullptr;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInformation")
	TArray<FFQFAbilityInfo> AbilityInformation;
	
	UFUNCTION(BlueprintCallable, Category="AbilityInformation")
	FFQFAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;

	
};
