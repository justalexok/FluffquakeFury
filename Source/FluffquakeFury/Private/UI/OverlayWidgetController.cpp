// Copyright Alex Goulder


#include "UI/OverlayWidgetController.h"
#include "AbilitySystem/FQFAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UFQFAttributeSet* FQFAttributeSet = CastChecked<UFQFAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(FQFAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(FQFAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(FQFAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(FQFAttributeSet->GetMaxMana());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UFQFAttributeSet* FQFAttributeSet = CastChecked<UFQFAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FQFAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FQFAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FQFAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FQFAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}
void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}
void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}