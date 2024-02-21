// Copyright Alex Goulder


#include "AbilitySystem/ExecCalc/ExecCalcJumpCost.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"

struct FQFJumpCostStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Strength);



	FQFJumpCostStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, Strength, Target, false);
	}
};

static const FQFJumpCostStatics& JumpCostStatics()
{
	static FQFJumpCostStatics DStatics;
	return DStatics;
}

UExecCalcJumpCost::UExecCalcJumpCost()
{
	RelevantAttributesToCapture.Add(FQFJumpCostStatics().StrengthDef);
}

void UExecCalcJumpCost::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                               FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Cost = 0.f;

	float Strength = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FQFJumpCostStatics().StrengthDef, EvaluationParameters, Strength);

	Cost = Strength * -0.1;

	const FGameplayModifierEvaluatedData IncomingDamageEvaluatedData(UFQFAttributeSet::GetStrengthAttribute(), EGameplayModOp::Additive, Cost);
	OutExecutionOutput.AddOutputModifier(IncomingDamageEvaluatedData);
}
