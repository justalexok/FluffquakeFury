// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FQFAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class APippaPlayerController;

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;

	UPROPERTY()
	FGameplayTag AttackTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UFQFAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//Vital
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Fluff, Category = "Vital Attributes")
	FGameplayAttributeData Fluff;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, Fluff);
	
	//Primary
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Primary Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxFluff, Category = "Primary Attributes")
	FGameplayAttributeData MaxFluff;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, MaxFluff);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_LoadedFluff, Category = "Primary Attributes")
	FGameplayAttributeData LoadedFluff;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, LoadedFluff);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, Strength);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, Resilience);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Primary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, BlockChance);

	//Meta
	
	UPROPERTY(BlueprintReadOnly, Category = "Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UFQFAttributeSet, IncomingDamage);

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_Fluff(const FGameplayAttributeData& OldFluff) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_MaxFluff(const FGameplayAttributeData& OldMaxFluff) const;

	UFUNCTION()
	void OnRep_LoadedFluff(const FGameplayAttributeData& OldLoadedFluff) const;
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	
private:
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	static void ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bPillowExploded, bool bFatalHit);
	void SpawnNiagara(ACharacter* SourceCharacter, float Damage) const;
	void HandleExplosion(const FEffectProperties& Props, float LocalIncomingDamage) const;
	void SetFluffLost(const FEffectProperties& Props, float LocalIncomingDamage) const;
	
};

