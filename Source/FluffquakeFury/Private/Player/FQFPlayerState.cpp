// Copyright Alex Goulder


#include "Player/FQFPlayerState.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
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

void AFQFPlayerState::OnRep_Level(int32 OldLevel)
{
}
