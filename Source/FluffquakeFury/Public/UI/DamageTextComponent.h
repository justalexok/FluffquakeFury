// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bPillowExploded, bool bFatalHit);


};
