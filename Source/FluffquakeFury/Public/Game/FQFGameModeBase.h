// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "GameFramework/GameModeBase.h"
#include "FQFGameModeBase.generated.h"

class AEffectActor;
class ULevelInfo;
class UAbilityInfo;
class UCharacterClassInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelCompletionDelegate);

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AFQFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FLevelCompletionDelegate OnLevelCompletion;
	
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Information")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Level Information")
	TObjectPtr<ULevelInfo> LevelInfo;

	UFUNCTION(BlueprintCallable)
	FFQFLevelInfo GetCurrentLevelInfo();
	
	UFUNCTION(BlueprintCallable)
	FName GetNextLevelName();

	UFUNCTION(BlueprintCallable)
	void CheckIfLevelComplete();
	
	UPROPERTY(EditAnywhere)
	int32 NumEnemiesInLevel = 0;

	UFUNCTION(BlueprintCallable)
	void GoToNextLevel();
		
};
