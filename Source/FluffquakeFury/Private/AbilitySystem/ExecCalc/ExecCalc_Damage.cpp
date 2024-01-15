// Copyright Alex Goulder


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"

struct FQFDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resilience);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LoadedFluff);

	FQFDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, Resilience, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, LoadedFluff, Source, false);

	}
};

static const FQFDamageStatics& DamageStatics()
{
	static FQFDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ResilienceDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LoadedFluffDef);

}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	//Get Damage Set By Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FFQFGameplayTags::Get().Damage);

	//Capture BlockChance on Target and determine if successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);	
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UFQFBlueprintFunctionLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	
	//Capture LoadedFluff on Source 
	float SourceLoadedFluff = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LoadedFluffDef, EvaluationParameters, SourceLoadedFluff);

	//Get ExplosionChance Set By Caller Magnitude and determine if explosion
	const float ExplosionChance = Spec.GetSetByCallerMagnitude(FFQFGameplayTags::Get().ExplosionChance);
	UE_LOG(LogTemp, Warning, TEXT("Explosion Chance : %f"), ExplosionChance);
	const bool bExploded = FMath::RandRange(1, 100) < ExplosionChance;

	UFQFBlueprintFunctionLibrary::SetHasPillowExploded(EffectContextHandle, bExploded);
	
	//Alter Damage
	Damage *= SourceLoadedFluff / 100;
	
	Damage = bBlocked ? Damage / 2.f : Damage;	

	if (bExploded)
	{
		Damage = 0;
		UE_LOG(LogTemp, Error, TEXT("PILLOW EXPLODED!"));

	}

	Damage = round(Damage);
	
	const FGameplayModifierEvaluatedData EvaluatedData(UFQFAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
