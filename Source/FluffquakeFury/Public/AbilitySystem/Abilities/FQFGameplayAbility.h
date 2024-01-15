// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "FQFGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	AActor* SourceWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FScalableFloat ExplosionChance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	FVector DamageImpactLocation;
	
	UFUNCTION(BlueprintCallable)
	void ApplyDamageToTargetActor(AActor* TargetActor);

	
};
