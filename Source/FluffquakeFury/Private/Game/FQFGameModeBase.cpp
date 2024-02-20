// Copyright Alex Goulder


#include "Game/FQFGameModeBase.h"

#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Character/FQFCharacterBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "Game/FQFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FQFPlayerState.h"
#include "Player/PippaPlayerController.h"

FFQFLevelInfo AFQFGameModeBase::GetCurrentLevelInfo()
{
	PippaCharacterBase = Cast<AFQFCharacterBase>(UGameplayStatics::GetPlayerPawn(this,0));
	if (PippaCharacterBase)
	{
		const int32 CurrentLevel = PippaCharacterBase->GetPlayerLevel();

		if (LevelInfo->LevelInformation.Num() > CurrentLevel) 
		{
			return LevelInfo->LevelInformation[CurrentLevel];
		}

	}
	return FFQFLevelInfo();
}

FFQFWorldInfoNew AFQFGameModeBase::GetCurrentWorldInfo()
{

	if (const UFQFGameInstance* GameInstance = UFQFBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (WorldInfo->WorldInformation.Num() <= GameInstance->CurrentWorldIndex)
		{
			UE_LOG(LogTemp,Error,TEXT("Tried to Find World Info at Index %d where there was none in Data Asset!"),GameInstance->CurrentWorldIndex);
			return FFQFWorldInfoNew();

		}

		const FFQFWorldInfoNew CurrentWorldInfo = WorldInfo->WorldInformation[GameInstance->CurrentWorldIndex];
		return CurrentWorldInfo;		
	}	
	return FFQFWorldInfoNew();
}

void AFQFGameModeBase::CheckIfLevelComplete()
{
	NumEnemiesInLevel = CountActiveEnemiesInWorldForLevelIndex(GetCurrentLevelInfo().LevelIndexInWorld);
	UE_LOG(LogTemp,Warning,TEXT("Number of Enemies in World: %d"),NumEnemiesInLevel);

	const APippaPlayerController* PlayerController = CastChecked<APippaPlayerController>(GetWorld()->GetFirstPlayerController());

	//Must be 0 enemies and have made it past the minimum survival length
	if (NumEnemiesInLevel == 0 && PlayerController->LevelSecondsRemaining < (GetCurrentLevelInfo().LevelLength - GetCurrentLevelInfo().MinimumSurvivalLength))
	{
		UE_LOG(LogTemp,Warning,TEXT("LEVEL COMPLETE!"))
		OnLevelCompletionDelegate.Broadcast();
	}

	
}

void AFQFGameModeBase::RestartCurrentWorld()
{
	if (UFQFGameInstance* GameInstance = UFQFBlueprintFunctionLibrary::GetGameInstance(this))
	{
		//Finds the Level we started this world at and resets the player level to that level both on the playerstate and game instance.
		int32 CurrentWorldIndex = GameInstance->GetCurrentWorldIndex();
		int32 Index = 0;
		for (FFQFLevelInfo Info : LevelInfo->LevelInformation)
		{
			if (Info.WorldIndex == CurrentWorldIndex)
			{
				GameInstance->SetCurrentLevel(Index);
				
				AFQFPlayerState* FQFPlayerState = UGameplayStatics::GetPlayerPawn(this,0)->GetPlayerState<AFQFPlayerState>();
				check(FQFPlayerState);
				FQFPlayerState->SetLevel(Index);
				break;
			}
			Index++;
		}
	}
	UGameplayStatics::OpenLevel(this,GetCurrentWorldInfo().WorldName);
	
}

void AFQFGameModeBase::AddAnyPreviouslyGrantedAbilities()
{
	TArray<TSubclassOf<UGameplayAbility>> PreviouslyGrantedAbilities;

	// Loops through Level info up till index of current level, adding any abilities granted on completion to Tarray
	//at end of loop, add those abilities.

	PippaCharacterBase = Cast<AFQFCharacterBase>(UGameplayStatics::GetPlayerPawn(this,0));
	if (PippaCharacterBase)
	{
		const int32 CurrentLevel = PippaCharacterBase->GetPlayerLevel();
		for (int32 Index = 0; Index <= CurrentLevel && Index < LevelInfo->LevelInformation.Num(); ++Index)
		{
			FFQFLevelInfo& Info = LevelInfo->LevelInformation[Index];
			PreviouslyGrantedAbilities.Add(Info.AbilityGrantedOnCompletion);
		}
	}
	
	if (UFQFAbilitySystemComponent* PippaASC = Cast<UFQFAbilitySystemComponent>(PippaCharacterBase->GetAbilitySystemComponent()))
	{
		for (const TSubclassOf<UGameplayAbility> Ability : PreviouslyGrantedAbilities)
		{
			PippaASC->AddAbility(Ability);
		}
	}

	if (UFQFGameInstance* GameInstance = UFQFBlueprintFunctionLibrary::GetGameInstance(this))
	{
		int32 CurrentLevel = GameInstance->CurrentLevel;
		UE_LOG(LogTemp,Error,TEXT("Game instance thinks the level is [%d]"),CurrentLevel);

		int32 CurrentWorldIndex = GameInstance->CurrentWorldIndex;
		UE_LOG(LogTemp,Error,TEXT("Game instance thinks the WorldIndex is [%d]"),CurrentWorldIndex);
	}
	
	
}

int32 AFQFGameModeBase::CountActiveEnemiesInWorldForLevelIndex(int32 Level)
{
	FString LevelString = FString::FromInt(Level);
	FName TagFName(*LevelString);

	TArray<AActor*> OutEnemies;
	UGameplayStatics::GetAllActorsOfClassWithTag(this,AEnemyBase::StaticClass(), TagFName,OutEnemies);

	int32 ActiveEnemyCount = 0;
	for (AActor* Enemy : OutEnemies)
	{
		if (!Cast<AEnemyBase>(Enemy)->Execute_IsDead(Enemy))
		{
			ActiveEnemyCount ++;
		}
	}
	return  ActiveEnemyCount;
}

int32 AFQFGameModeBase::CountNumLevelsInCurrentWorld()
{
	return LevelInfo->CountLevelsInWorld(GetCurrentLevelInfo().WorldIndex);
}

	
