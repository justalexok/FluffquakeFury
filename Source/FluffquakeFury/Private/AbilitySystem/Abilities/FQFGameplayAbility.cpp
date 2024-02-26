// Copyright Alex Goulder


#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/FQFCharacterBase.h"
#include "Character/Pippa/PippaCharacter.h"


FDamageEffectParams UFQFGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,  FVector InRadialDamageOrigin, bool bDidJumpOffBed) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UE_LOG(LogTemp,Warning, TEXT("Damage At Ability Level: %f"),Params.BaseDamage);

	if (const APippaCharacter* PippaCharacter = Cast<APippaCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (const UFQFAttributeSet* AttributeSet = UFQFBlueprintFunctionLibrary::GetAttributeSet(PippaCharacter))
		{
			if (DamageType == FFQFGameplayTags::Get().DamageType_Physical)
			{
				Params.BaseDamage += Damage.GetValueAtLevel(AttributeSet->GetStrength());
				if (bDidJumpOffBed) { Params.BaseDamage *= 3 ;}
				UE_LOG(LogTemp,Warning, TEXT("Adding to damage: %f"),Damage.GetValueAtLevel(AttributeSet->GetStrength()));
			}
			if (DamageType == FFQFGameplayTags::Get().DamageType_Fluff)
			{
				Params.ExplosionChance = ExplosionChance.GetValueAtLevel(AttributeSet->GetLoadedFluff());
				Params.BaseDamage *= LoadedFluffCoefficient.GetValueAtLevel(AttributeSet->GetLoadedFluff());
			}
		}
			
	}		
	
	UE_LOG(LogTemp,Warning, TEXT("Damage After Adjustments in Gameplay Ability : %f"),Params.BaseDamage);
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = bDidJumpOffBed ? KnockbackForceMagnitude * 5 : KnockbackForceMagnitude;
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
