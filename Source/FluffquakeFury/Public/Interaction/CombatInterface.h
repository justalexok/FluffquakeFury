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


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
// USTRUCT(BlueprintType)
// struct FTaggedMontage
// {
// 	GENERATED_BODY()
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	UAnimMontage* AnimMontage = nullptr;
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	FGameplayTag MontageTag;
//
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	FGameplayTag SocketTag;
// 	
// 	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
// 	USoundBase* ImpactSound = nullptr;
// };


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
	
	virtual void Die() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDead() const;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsDefending();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void KnockbackCharacter(float Magnitude, float Pitch, FVector Direction);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetMaxWalkSpeed(bool bShouldImmobiliseCharacter);

	virtual FOnDeathSignature& GetOnDeathDelegate() = 0;

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
