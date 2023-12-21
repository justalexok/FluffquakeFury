// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FQFHUD.generated.h"

class UFQFUserWidget;
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AFQFHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TObjectPtr<UFQFUserWidget>  OverlayWidget;

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFQFUserWidget> OverlayWidgetClass;	
};
