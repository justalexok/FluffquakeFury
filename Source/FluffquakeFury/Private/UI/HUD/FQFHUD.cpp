// Copyright Alex Goulder


#include "UI/HUD/FQFHUD.h"
#include "UI/FQFUserWidget.h"


void AFQFHUD::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
