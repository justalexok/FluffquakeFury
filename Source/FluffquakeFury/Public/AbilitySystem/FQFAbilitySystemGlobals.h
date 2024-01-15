// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "FQFAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

};
