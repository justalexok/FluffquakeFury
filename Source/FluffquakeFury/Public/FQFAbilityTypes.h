#pragma once

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

	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetHasPillowExploded(bool bInHasPillowExploded) { bHasPillowExploded = bInHasPillowExploded; }

	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }

	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	float GetKnockbackChance() const { return KnockbackChance; }

	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }
	void SetKnockbackChance(const float InChance) { KnockbackChance = InChance; }

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
	
	TSharedPtr<FGameplayTag> DamageType;
	
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	float KnockbackChance = 0;
	
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

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;


	// UPROPERTY()
	// bool bIsRadialDamage = false;
	//
	// UPROPERTY()
	// float DistanceToTarget = 0.f;
	

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();


};
