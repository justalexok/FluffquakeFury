// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FQFUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;
	
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
