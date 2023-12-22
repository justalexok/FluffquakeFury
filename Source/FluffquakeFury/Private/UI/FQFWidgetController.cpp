// Copyright Alex Goulder


#include "UI/FQFWidgetController.h"

void UFQFWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UFQFWidgetController::BroadcastInitialValues()
{
}

void UFQFWidgetController::BindCallbacksToDependencies()
{
}
