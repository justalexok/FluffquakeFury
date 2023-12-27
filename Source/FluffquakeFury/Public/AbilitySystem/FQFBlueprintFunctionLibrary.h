// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FQFBlueprintFunctionLibrary.generated.h"

class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="FQFBlueprintFunctionLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
};
