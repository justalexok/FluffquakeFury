// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "AbilitySystem/Data/WorldInfoNew.h"
#include "GameFramework/GameModeBase.h"
#include "FQFGameModeBase.generated.h"

class AFQFCharacterBase;
class AEffectActor;
class ULevelInfo;
class UAbilityInfo;
class UCharacterClassInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGeneralLevelMechanicsSignature);



/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AFQFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UPROPERTY()
	AFQFCharacterBase* PippaCharacterBase;

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FGeneralLevelMechanicsSignature OnLevelCompletionDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FGeneralLevelMechanicsSignature OnWorldCompletionDelegate;
	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastWorldCompletion() const {OnWorldCompletionDelegate.Broadcast();}

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FGeneralLevelMechanicsSignature OnLevelFailureDelegate;
	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastLevelFailure() const {OnLevelFailureDelegate.Broadcast();}	

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FGeneralLevelMechanicsSignature OnPlayerDeathDelegate;
	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastPlayerDeath() const {OnPlayerDeathDelegate.Broadcast();}
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Information")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Information")
	TObjectPtr<ULevelInfo> LevelInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Information")
	TObjectPtr<UWorldInfoNew> WorldInfo;

	UFUNCTION(BlueprintCallable)
	FFQFLevelInfo GetCurrentLevelInfo();
	
	UFUNCTION(BlueprintCallable)
	FFQFWorldInfoNew GetCurrentWorldInfo();
	
	UFUNCTION(BlueprintCallable)
	void CheckIfLevelComplete();
	
	UPROPERTY(EditAnywhere)
	int32 NumEnemiesInLevel = 0;

	UFUNCTION(BlueprintCallable)
	void RestartCurrentWorld();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsLevelComplete = false;

	UFUNCTION(BlueprintCallable)
	void AddAnyPreviouslyGrantedAbilities();

	UFUNCTION()
	int32 CountActiveEnemiesInWorldForLevelIndex(int32 Level);

	UFUNCTION(BlueprintCallable)
	int32 CountNumLevelsInCurrentWorld();
};
