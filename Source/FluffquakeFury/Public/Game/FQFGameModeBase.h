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


UENUM(BlueprintType)
enum class EHotelLocation : uint8
{
	None,
	Room1,
	Room2,
	Room3,
	Room4,
	Landing,
	Lift,
	Basement
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGeneralLevelMechanicsSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnemyDeathSignature, EHotelLocation, HotelLocation);



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
	UFUNCTION(BlueprintCallable, Category = "Events")
	void BroadcastLevelCompletion() const {OnLevelCompletionDelegate.Broadcast();}

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

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FEnemyDeathSignature OnEnemyDeathDelegate;
	
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

	UFUNCTION(BlueprintCallable)
	int32 NumLevelsInPreviousWorlds();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnReward();

	UPROPERTY(BlueprintReadWrite)
	bool HotelLevelFinishedSpawning = true;
};
