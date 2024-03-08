// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UNiagaraSystem;
class UAnimMontage;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageSignature, float /*DamageAmount*/);


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};


class FLUFFQUAKEFURY_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& SpecificAbilityTag);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetFacingTarget(const FVector& FacingTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();



	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetRecentlyReceivedDamageType();
	
	virtual void Die(const FVector& DeathImpulse) = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDefending();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void KnockbackCharacter(float Magnitude, float Pitch, AActor* Target, AActor* Subject);

	UFUNCTION(BlueprintNativeEvent)
	void ExplodePillow();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMaxWalkSpeed(bool bShouldImmobiliseCharacter);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMaxWalkSpeedByFloat(float InWalkSpeed);
	
	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;
	virtual FOnDamageSignature& GetOnDamageSignature() = 0; 

	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimInstance* GetAnimInstance();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetBlackboardBoolForKey(FName KeyName, bool Bool);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetActorGroundPoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetActorHaloPoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetWeaponVisibility(bool bVisible);
};
