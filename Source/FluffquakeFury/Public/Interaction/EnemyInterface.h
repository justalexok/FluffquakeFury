// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FLUFFQUAKEFURY_API IEnemyInterface
{
	GENERATED_BODY()
public:
	virtual void HighlightActor() = 0;
	virtual void UnHighlightActor() = 0;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EnemyHandleLevelShouldBegin();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FLinearColor GetEnemyColor();
	
};
