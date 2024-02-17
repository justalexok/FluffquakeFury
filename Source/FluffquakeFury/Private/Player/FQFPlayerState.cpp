// Copyright Alex Goulder


#include "Player/FQFPlayerState.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/Enemy/EnemyBase.h"
#include "Game/FQFGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AFQFPlayerState::AFQFPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFQFAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UFQFAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;

	
}

void AFQFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFQFPlayerState, Level);

}

void AFQFPlayerState::AddToLevel(int32 InLevel)
{
	UFQFGameInstance* GameInstance = UFQFBlueprintFunctionLibrary::GetGameInstance(this);
	if (GameInstance)
	{
		GameInstance->AddToLevel(InLevel);
	}
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);	
}

void AFQFPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

TArray<AActor*> AFQFPlayerState::GetEnemyActorsInLevel()
{
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(this, EnemyClass, FName("Enemy"),EnemyActors);

	return EnemyActors;
}

float AFQFPlayerState::SumHealthOfAllEnemiesRemaining()
{
	TArray<AActor*> EnemyActors = GetEnemyActorsInLevel();

	float SumHealth = 0;
	for (AActor* Actor : EnemyActors)
	{
		if (AFQFCharacterBase* CharacterBase = Cast<AFQFCharacterBase>(Actor))
		{
			if (UFQFAttributeSet* AS = Cast<UFQFAttributeSet>(CharacterBase->GetAttributeSet()))
			{
				SumHealth += AS->GetHealth();
			}
		}
	}
	return SumHealth;
}

void AFQFPlayerState::OnRep_Level(int32 OldLevel) const
{
	OnLevelChangedDelegate.Broadcast(Level);
}
