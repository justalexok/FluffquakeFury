// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "UI/FQFWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UFQFUserWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelFailureSignature);



/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FLUFFQUAKEFURY_API UOverlayWidgetController : public UFQFWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnFluffChanged;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxFluffChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnLoadedFluffChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="GameplayMechanics")
	FOnLevelFailureSignature OnLevelFailureDelegate;

	UPROPERTY(BlueprintReadOnly)
	float SumEnemyHealthAtStartOfLevel = 0.f;

	
protected:
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	void OnXPChanged(int32 NewXP) const;

};


