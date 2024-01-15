// Copyright Alex Goulder


#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FQFGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"

void UFQFGameplayAbility::ApplyDamageToTargetActor(AActor* TargetActor)
{
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

		const FFQFGameplayTags GameplayTags = FFQFGameplayTags::Get();

		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);

		UFQFAttributeSet* AttributeSet = UFQFBlueprintFunctionLibrary::GetAttributeSet(this);
		const float ScaledExplosionChance = ExplosionChance.GetValueAtLevel(AttributeSet->GetLoadedFluff());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.ExplosionChance, ScaledExplosionChance);

		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
	}
}
