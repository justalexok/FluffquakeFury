// Copyright Alex Goulder


#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/FQFCharacterBase.h"


FDamageEffectParams UFQFGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,  FVector InRadialDamageOrigin) const
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
		if (DamageType == FFQFGameplayTags::Get().DamageType_Physical)
		{
			Params.BaseDamage += Damage.GetValueAtLevel(AttributeSet->GetStrength());
		}
	}
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackForce = bIsRadialDamage ? ToTarget * KnockbackForceMagnitude * Params.BaseDamage/500: ToTarget * KnockbackForceMagnitude;

	}
	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		UE_LOG(LogTemp,Warning, TEXT("Knockback Magnitude = %f, Params.BaseDamage = %f"), KnockbackForceMagnitude, Params.BaseDamage);
		Params.RadialDamageOrigin = InRadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	return  Params;
}
