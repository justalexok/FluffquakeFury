// Copyright Alex Goulder


#include "Game/FQFGameModeBase.h"

#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelInfo.h"
#include "Character/FQFCharacterBase.h"
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

void AFQFGameModeBase::CheckIfLevelComplete()
{
	
	UE_LOG(LogTemp,Warning,TEXT("Number of Enemies in World: %d"),NumEnemiesInLevel);

	APippaPlayerController* PlayerController = CastChecked<APippaPlayerController>(GetWorld()->GetFirstPlayerController());

	//Must be 0 enemies and have made it past the minimum survival length
	if (NumEnemiesInLevel == 0 && PlayerController->LevelSecondsRemaining < (GetCurrentLevelInfo().LevelLength - GetCurrentLevelInfo().MinimumSurvivalLength))
	{
		UE_LOG(LogTemp,Warning,TEXT("LEVEL CAN BE COMPLETE!"))
		LevelCanBeCompletedDelegate.Broadcast();
	}

	
}


void AFQFGameModeBase::RestartCurrentLevel()
{
	// bIsLevelComplete = false;
	UGameplayStatics::OpenLevel(this,GetCurrentLevelInfo().LevelName);
}

void AFQFGameModeBase::AddAnyPreviouslyGrantedAbilities()
{
	TArray<TSubclassOf<UGameplayAbility>> PreviouslyGrantedAbilities;

	// Loops through Level info, if same level as current level, then breaks out of loop, otherwise adds ability granted on completion to Tarray
	//at end of loop, add those abilities.

	for (FFQFLevelInfo Info : LevelInfo->LevelInformation)
	{
		if (Info.LevelName == GetCurrentLevelInfo().LevelName)
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
	
	
}
