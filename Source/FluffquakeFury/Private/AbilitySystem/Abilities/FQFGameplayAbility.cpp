// Copyright Alex Goulder


#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UFQFGameplayAbility::ApplyDamageToTargetActor(AActor* TargetActor)
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		const FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), TargetASC->MakeEffectContext());
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}
