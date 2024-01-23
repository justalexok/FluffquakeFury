﻿#pragma once

#include "GameplayEffectTypes.h"
#include "FQFAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FFQFGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	
public:

	bool IsBlockedHit () const { return bIsBlockedHit; }
	bool HasPillowExploded () const { return bHasPillowExploded; }
	// bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetHasPillowExploded(bool bInHasPillowExploded) { bHasPillowExploded = bInHasPillowExploded; }
	// void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FFQFGameplayEffectContext* Duplicate() const
	{
		FFQFGameplayEffectContext* NewContext = new FFQFGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}


	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;
	
	UPROPERTY()
	bool bHasPillowExploded = false;

	// UPROPERTY()
	// bool bIsCriticalHit = false;
};
	template<>
	struct TStructOpsTypeTraits<FFQFGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FFQFGameplayEffectContext>
	{
		enum
		{
			WithNetSerializer = true,
			WithCopy = true
		};
};

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float ExplosionChance = 0.f;

	// UPROPERTY()
	// AActor* SourceWeapon

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

};
