// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FQFAIController.generated.h"
class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AFQFAIController : public AAIController
{
	GENERATED_BODY()
public:
	AFQFAIController();
protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
