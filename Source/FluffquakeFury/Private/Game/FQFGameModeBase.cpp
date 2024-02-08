// Copyright Alex Goulder


#include "Game/FQFGameModeBase.h"

#include "AbilitySystem/Data/LevelInfo.h"
#include "Character/FQFCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PippaPlayerController.h"


FFQFLevelInfo AFQFGameModeBase::GetCurrentLevelInfo()
{
	if (AFQFCharacterBase* CharacterBase = Cast<AFQFCharacterBase>(UGameplayStatics::GetPlayerPawn(this,0)))
	{
		const int32 CurrentLevel = CharacterBase->GetPlayerLevel();

		return LevelInfo->LevelInformation[CurrentLevel];
	}
	return FFQFLevelInfo();
}

FName AFQFGameModeBase::GetNextLevelName()
{
	if (AFQFCharacterBase* CharacterBase = Cast<AFQFCharacterBase>(UGameplayStatics::GetPlayerPawn(this,0)))
	{
		const int32 CurrentLevel = CharacterBase->GetPlayerLevel();

		const FFQFLevelInfo NextLevelInfo = LevelInfo->LevelInformation[CurrentLevel + 1];

		return NextLevelInfo.LevelName;
	}
	return FName("");
}

void AFQFGameModeBase::CheckIfLevelComplete()
{
	
	UE_LOG(LogTemp,Warning,TEXT("Number of Enemies in World: %d"),NumEnemiesInLevel);

	APippaPlayerController* PlayerController = CastChecked<APippaPlayerController>(GetWorld()->GetFirstPlayerController());

	//Must be 0 enemies and have made it past the minimum survival length
	if (NumEnemiesInLevel == 0 && PlayerController->LevelSecondsRemaining < GetCurrentLevelInfo().MinimumSurvivalLength)
	{
		UE_LOG(LogTemp,Warning,TEXT("LEVEL COMPLETE!"))
		OnLevelCompletion.Broadcast();
	}

	
}

void AFQFGameModeBase::GoToNextLevel()
{
	UGameplayStatics::OpenLevel(this,GetNextLevelName());
}

void AFQFGameModeBase::RestartCurrentLevel()
{
	UGameplayStatics::OpenLevel(this,GetCurrentLevelInfo().LevelName);
}
