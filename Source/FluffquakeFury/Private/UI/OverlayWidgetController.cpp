// Copyright Alex Goulder


#include "UI/OverlayWidgetController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Player/FQFPlayerState.h"
#include "Player/PippaPlayerController.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UFQFAttributeSet* FQFAttributeSet = CastChecked<UFQFAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(FQFAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(FQFAttributeSet->GetMaxHealth());
	OnFluffChanged.Broadcast(FQFAttributeSet->GetFluff());
	OnMaxFluffChanged.Broadcast(FQFAttributeSet->GetMaxFluff());

	if (AFQFPlayerState* PS = Cast<AFQFPlayerState>(PlayerState))
	{
		PS->SumHealthOfEnemiesAtStartOfLevel = PS->SumHealthOfAllEnemiesRemaining();
		SumEnemyHealthAtStartOfLevel = PS->SumHealthOfEnemiesAtStartOfLevel;
		UE_LOG(LogTemp,Warning, TEXT("Sum of All Enemies in Level Health = %f"),PS->SumHealthOfEnemiesAtStartOfLevel);
	}

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

	AFQFPlayerState* FQFPlayerState = CastChecked<AFQFPlayerState>(PlayerState);

	FQFPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		});

}

void UOverlayWidgetController::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

