// Copyright Alex Goulder


#include "UI/FQFUserWidget.h"

void UFQFUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
