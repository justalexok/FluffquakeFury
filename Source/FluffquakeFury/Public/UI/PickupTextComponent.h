// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "PickupTextComponent.generated.h"

struct FGameplayTag;
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UPickupTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetPickupText(float Amount, FGameplayTag AttributeTag);
};
