// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "GameFramework/GameModeBase.h"
#include "FQFGameModeBase.generated.h"

class AFQFCharacterBase;
class AEffectActor;
class ULevelInfo;
class UAbilityInfo;
class UCharacterClassInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelCompletionSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelCanBeCompletedSignature);



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
	FLevelCompletionSignature OnLevelCompletionDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FLevelCanBeCompletedSignature LevelCanBeCompletedDelegate;

	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Information")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Information")
	TObjectPtr<ULevelInfo> LevelInfo;

	UFUNCTION(BlueprintCallable)
	FFQFLevelInfo GetCurrentLevelInfo();
	
	UFUNCTION(BlueprintCallable)
	void CheckIfLevelComplete();
	
	UPROPERTY(EditAnywhere)
	int32 NumEnemiesInLevel = 0;

	UFUNCTION(BlueprintCallable)
	void GoToNextLevel();

	UFUNCTION(BlueprintCallable)
	void RestartCurrentLevel();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsLevelComplete = false;

	UFUNCTION(BlueprintCallable)
	void AddAnyPreviouslyGrantedAbilities();
		
};
