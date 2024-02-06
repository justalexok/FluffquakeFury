// Copyright Alex Goulder


#include "Game/FQFGameModeBase.h"

#include "AbilitySystem/Data/LevelInfo.h"
#include "Character/FQFCharacterBase.h"
#include "Kismet/GameplayStatics.h"

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
