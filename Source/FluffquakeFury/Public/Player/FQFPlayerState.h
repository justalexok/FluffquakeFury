// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "FQFPlayerState.generated.h"

class AEnemyBase;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32,StatValue);

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AFQFPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFQFPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnPlayerStatChanged OnLevelChangedDelegate;
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

	void AddToLevel(int32 InLevel);
	
	UFUNCTION(BlueprintCallable)
	void SetLevel(int32 InLevel);
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*>GetEnemyActorsInLevel();

	UFUNCTION(BlueprintCallable)
	float SumHealthOfAllEnemiesRemaining();

	UPROPERTY()
	float SumHealthOfEnemiesAtStartOfLevel = 0;

protected:

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyBase> EnemyClass;
	
private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;


	
};
