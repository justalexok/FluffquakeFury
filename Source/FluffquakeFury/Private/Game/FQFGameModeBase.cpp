// Copyright Alex Goulder


#include "Game/FQFGameModeBase.h"

#include "AbilitySystem/Data/LevelInfo.h"
#include "Character/FQFCharacterBase.h"
#include "Character/Enemy/EnemyBase.h"
#include "Kismet/GameplayStatics.h"

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

bool AFQFGameModeBase::IsLevelComplete()
{
	//is time elapsed > MinimumSurvivalLength
	//and are there zero enemies in the level?
	// GetCurrentLevelInfo().MinimumSurvivalLength

	
	

	UE_LOG(LogTemp,Warning,TEXT("Number of Enemies in World: %d"),NumEnemiesInLevel);
	
	if (NumEnemiesInLevel == 0 && 10 < GetCurrentLevelInfo().MinimumSurvivalLength)
	{
		UE_LOG(LogTemp,Warning,TEXT("LEVEL COMPLETE!"))
		return true;
	}
	UE_LOG(LogTemp,Error,TEXT("LEVEL INCOMPLETE!"))

	return false;
	
}
