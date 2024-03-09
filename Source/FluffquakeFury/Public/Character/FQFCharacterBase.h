// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Actor/PillowBase.h"
#include "Game/FQFGameModeBase.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "FQFCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;


	
UCLASS()
class FLUFFQUAKEFURY_API AFQFCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AFQFCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override {return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	//Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SpecificAbilityTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void SetMaxWalkSpeed_Implementation(bool bShouldImmobiliseCharacter) override;
	virtual void SetMaxWalkSpeedByFloat_Implementation(float InWalkSpeed) override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual void ResetRecentlyReceivedDamageType_Implementation() override;
	virtual UAnimInstance* GetAnimInstance_Implementation() override;
	virtual FVector GetActorGroundPoint_Implementation() override;
	virtual FVector GetActorHaloPoint_Implementation() override;
	virtual void SetWeaponVisibility_Implementation(bool bVisible) override;
	virtual void KnockbackCharacter_Implementation(float Magnitude, float Pitch, AActor* Target, AActor* Subject) override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;

	
	UFUNCTION()
	void HandleDeath();

	UFUNCTION(BlueprintCallable)
	void RagdollMesh();

	FOnDeathSignature OnDeathDelegate;

	FOnDamageSignature OnDamageDelegate;


	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<APillowBase> PillowClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Default")
	APillowBase* Pillow;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Default")
	FLinearColor CharacterColor;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag RecentlyReceivedDamageType = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveInfiniteEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveDurationEffectHandles;

	UFUNCTION(BlueprintCallable)
	void RemoveInfiniteGameplayEffects();
	
	UFUNCTION(BlueprintCallable)
	void RemoveDurationEffectsOnDeath();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> StoredEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	bool bDead = false;
	



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseWalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* AudioComponent;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	
	virtual void InitializeDefaultAttributes() const;

	/* Dissolve Effects */

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

private:

	UPROPERTY(EditAnywhere, Category = "Combat")
	TMap<FGameplayTag,UAnimMontage*> DamageTypesToHitReactMontages;
};