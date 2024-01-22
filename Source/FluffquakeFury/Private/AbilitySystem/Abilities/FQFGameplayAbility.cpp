// Copyright Alex Goulder


#include "AbilitySystem/Abilities/FQFGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "FQFGameplayTags.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/FQFCharacterBase.h"
#include "GameplayEffect.h"
#include "ActiveGameplayEffectHandle.h"


void UFQFGameplayAbility::ApplyDamageToTargetActor(AActor* TargetActor)
{

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

		if (!IsValid(TargetActor)) return;
		if (!UFQFBlueprintFunctionLibrary::IsNotFriend(SourceASC->GetAvatarActor(), TargetActor)) return;	

		SetRecentlyReceivedDamageTag(TargetActor);
		
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

		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		//For infinite effects, add the active effect handle and targetASC to a map on the Sources Character.
		//To be removed on Source Death. 
		if (SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite)
		{
			if (AFQFCharacterBase* FQFCharacter = Cast<AFQFCharacterBase>(SourceASC->GetAvatarActor()))
			{
				FQFCharacter->ActiveInfiniteEffectHandles.Add(ActiveEffectHandle, TargetASC);
			}
		}
		
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

void UFQFGameplayAbility::SetRecentlyReceivedDamageTag(AActor* TargetActor)
{
	if (AFQFCharacterBase* TargetCharacter = Cast<AFQFCharacterBase>(TargetActor))
	{
		TargetCharacter->RecentlyReceivedDamageType = DamageType;
	}
}
