// Copyright Alex Goulder


#include "AbilitySystem/FQFAbilitySystemGlobals.h"
#include "FQFAbilityTypes.h"

FGameplayEffectContext* UFQFAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FFQFGameplayEffectContext();
}
