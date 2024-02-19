// Copyright Alex Goulder


#include "Game/FQFGameModeBase.h"

#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Character/FQFCharacterBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "Game/FQFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PippaPlayerController.h"

FFQFLevelInfo AFQFGameModeBase::GetCurrentLevelInfo()
{
	PippaCharacterBase = Cast<AFQFCharacterBase>(UGameplayStatics::GetPlayerPawn(this,0));
	if (PippaCharacterBase)
	{
		const int32 CurrentLevel = PippaCharacterBase->GetPlayerLevel();

		if (LevelInfo->LevelInformation.Num() >= CurrentLevel) 
		{
			return LevelInfo->LevelInformation[CurrentLevel];
		}

	}
	return FFQFLevelInfo();
}

FFQFWorldInfoNew AFQFGameModeBase::GetCurrentWorldInfo()
{

	if (UFQFGameInstance* GameInstance = UFQFBlueprintFunctionLibrary::GetGameInstance(this))
	{
		if (WorldInfo->WorldInformation.Num() <= GameInstance->CurrentWorldIndex)
		{
			UE_LOG(LogTemp,Error,TEXT("Tried to Find World Info at Index %d where there was none in Data Asset!"),GameInstance->CurrentWorldIndex);
			return FFQFWorldInfoNew();

		}
	
		FFQFWorldInfoNew CurrentWorldInfo = WorldInfo->WorldInformation[GameInstance->CurrentWorldIndex];
		return CurrentWorldInfo;
		
	}
	
	return FFQFWorldInfoNew();
}

void AFQFGameModeBase::CheckIfLevelComplete()
{
	

	NumEnemiesInLevel = CountActiveEnemiesInWorldForLevelIndex(GetCurrentLevelInfo().LevelIndex);
	UE_LOG(LogTemp,Warning,TEXT("Number of Enemies in World: %d"),NumEnemiesInLevel);

	APippaPlayerController* PlayerController = CastChecked<APippaPlayerController>(GetWorld()->GetFirstPlayerController());

	//Must be 0 enemies and have made it past the minimum survival length
	if (NumEnemiesInLevel == 0 && PlayerController->LevelSecondsRemaining < (GetCurrentLevelInfo().LevelLength - GetCurrentLevelInfo().MinimumSurvivalLength))
	{
		UE_LOG(LogTemp,Warning,TEXT("LEVEL CAN BE COMPLETE!"))
		OnLevelCanBeCompletedDelegate.Broadcast();
	}

	
}

void AFQFGameModeBase::GoToNextLevel()
{
	OnLevelCompletionDelegate.Broadcast();

}

void AFQFGameModeBase::RestartCurrentWorld()
{
	UGameplayStatics::OpenLevel(this,GetCurrentWorldInfo().WorldName);
}

void AFQFGameModeBase::AddAnyPreviouslyGrantedAbilities()
{
	TArray<TSubclassOf<UGameplayAbility>> PreviouslyGrantedAbilities;

	// Loops through Level info, if same level as current level, then breaks out of loop, otherwise adds ability granted on completion to Tarray
	//at end of loop, add those abilities.

	for (FFQFLevelInfo Info : LevelInfo->LevelInformation)
	{
		if (Info.LevelIndex == GetCurrentLevelInfo().LevelIndex)
		{
			break;
		}
		PreviouslyGrantedAbilities.Add(Info.AbilityGrantedOnCompletion);
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

	
