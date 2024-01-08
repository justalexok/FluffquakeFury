// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "FQFPillowAttack.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFPillowAttack : public UFQFGameplayAbility
{
	GENERATED_BODY()

	

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
