// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "Actor/PillowBase.h"
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
	
	//Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SpecificAbilityTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual void SetMaxWalkSpeed_Implementation(bool bShouldImmobiliseCharacter) override;
	// virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	// virtual FTaggedMontage GetAttackMontageWithTag_Implementation(FGameplayTag MontageTag) override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual void ResetRecentlyReceivedDamageType_Implementation() override;
	virtual UAnimInstance* GetAnimInstance_Implementation(AActor* Actor) override;
	virtual FVector GetActorGroundPoint_Implementation() override;
	virtual FVector GetActorHaloPoint_Implementation() override;
	virtual void SetWeaponVisibility_Implementation(bool bVisible) override;
	//
	// UPROPERTY(EditAnywhere, Category="Combat")
	// TArray<FTaggedMontage> AttackMontages;
	
	UFUNCTION()
	void HandleDeath();

	FOnDeathSignature OnDeathDelegate;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<APillowBase> PillowClass;

	APillowBase* Pillow;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag RecentlyReceivedDamageType = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveInfiniteEffectHandles;

protected:
	virtual void BeginPlay() override;
	
	// UPROPERTY(EditAnywhere)
	// TObjectPtr<USkeletalMeshComponent> TestPillowMesh;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	//
	// UPROPERTY(EditAnywhere, Category = "Combat")
	// FName WeaponTipSocketName;
	// UPROPERTY(EditAnywhere, Category = "Combat")
	// FName CenterChestSocketName;

	bool bDead = false;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

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