// Copyright Alex Goulder


#include "Player/FQFPlayerState.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"

AFQFPlayerState::AFQFPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFQFAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UFQFAttributeSet>("AttributeSet");
	
	NetUpdateFrequency = 100.f;

	
}
