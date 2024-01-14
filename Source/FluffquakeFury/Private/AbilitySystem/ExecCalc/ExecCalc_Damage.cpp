// Copyright Alex Goulder


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"

struct FQFDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resilience);

	FQFDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, Resilience, Target, false);
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

	float Resilience = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ResilienceDef, EvaluationParameters, Resilience);
	Resilience = FMath::Max<float>(0.f, Resilience);
	++Resilience;

	const FGameplayModifierEvaluatedData EvaluatedData(DamageStatics().ResilienceProperty, EGameplayModOp::Additive, Resilience);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	

}
