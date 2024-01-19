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
		FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
		EffectContextHandle.SetAbility(this);
		EffectContextHandle.AddSourceObject(SourceWeapon);
		FHitResult HitResult;
		HitResult.Location = DamageImpactLocation;
		EffectContextHandle.AddHitResult(HitResult);
		
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContextHandle);

		const FFQFGameplayTags GameplayTags = FFQFGameplayTags::Get();

		const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		// UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Damage, ScaledDamage);
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, ScaledDamage);


		if (DamageType == GameplayTags.DamageType_Fluff)
		{
			if (const UFQFAttributeSet* AttributeSet = UFQFBlueprintFunctionLibrary::GetAttributeSet(this))
			{
				const float ScaledExplosionChance = ExplosionChance.GetValueAtLevel(AttributeSet->GetLoadedFluff());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.ExplosionChance, ScaledExplosionChance);
			}
		}		

		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
	}
}

FTaggedMontage UFQFGameplayAbility::GetRandomTaggedMontage(TArray<FTaggedMontage> TaggedMontages)
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 RandomIndex = FMath::RandRange(0,TaggedMontages.Num()-1);
		return TaggedMontages[RandomIndex];
	}
	
	return FTaggedMontage(); 
}
