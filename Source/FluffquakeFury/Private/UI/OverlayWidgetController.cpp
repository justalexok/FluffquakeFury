// Copyright Alex Goulder


#include "UI/OverlayWidgetController.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UFQFAttributeSet* FQFAttributeSet = CastChecked<UFQFAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(FQFAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(FQFAttributeSet->GetMaxHealth());
	OnFluffChanged.Broadcast(FQFAttributeSet->GetFluff());
	OnMaxFluffChanged.Broadcast(FQFAttributeSet->GetMaxFluff());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UFQFAttributeSet* FQFAttributeSet = CastChecked<UFQFAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		FQFAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	FQFAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	FQFAttributeSet->GetFluffAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnFluffChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	FQFAttributeSet->GetMaxFluffAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxFluffChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
	FQFAttributeSet->GetLoadedFluffAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnLoadedFluffChanged.Broadcast(Data.NewValue);
		});

	Cast<UFQFAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{

			for (const FGameplayTag& Tag: AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}

			}
		});
}
