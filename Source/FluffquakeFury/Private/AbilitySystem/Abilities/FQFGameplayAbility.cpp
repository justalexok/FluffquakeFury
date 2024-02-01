// Copyright Alex Goulder


#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/FQFCharacterBase.h"



FTaggedMontage UFQFGameplayAbility::GetRandomTaggedMontage(TArray<FTaggedMontage> TaggedMontages)
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0,TaggedMontages.Num()-1);
		return TaggedMontages[RandomIndex];
	}
	
	return FTaggedMontage(); 
}

FDamageEffectParams UFQFGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	if (const UFQFAttributeSet* AttributeSet = UFQFBlueprintFunctionLibrary::GetAttributeSet(this))
	{
		Params.ExplosionChance = ExplosionChance.GetValueAtLevel(AttributeSet->GetLoadedFluff());
	}
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	return  Params;
}
