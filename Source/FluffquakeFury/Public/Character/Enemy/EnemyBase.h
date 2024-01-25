// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Character/FQFCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemyBase.generated.h"

class UBehaviorTree;
class AFQFAIController;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AEnemyBase : public AFQFCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()


public:
	AEnemyBase();
	virtual void PossessedBy(AController* NewController) override;

	//EnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;

	//CombatInterface
	virtual int32 GetPlayerLevel() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;


	virtual void Die() override;
	void RemoveInfiniteGameplayEffects();

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AFQFAIController> FQFAIController;

	UPROPERTY(EditAnywhere, Category = "Character Class Defaults")
	TArray<FGameplayTag>Immunities;

	UFUNCTION(BlueprintCallable)
	void SetIsCastingBlackboard(bool bIsCasting);
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	
};
