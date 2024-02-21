// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalcJumpCost.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UExecCalcJumpCost : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalcJumpCost();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
