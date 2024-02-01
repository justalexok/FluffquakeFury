// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "FQFAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "Interaction/CombatInterface.h"
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
	FGameplayTag SpecificAbilityTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	AActor* SourceWeapon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	FScalableFloat ExplosionChance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	FVector DamageImpactLocation;
	
	
	UFUNCTION(BlueprintPure)
	static FTaggedMontage GetRandomTaggedMontage(TArray<FTaggedMontage> TaggedMontages);

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	
};
