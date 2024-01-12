// Copyright Alex Goulder

#include "Actor/EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "UI/OverlayWidgetController.h"

// Sets default values
AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC == nullptr) return false;

	const UFQFAttributeSet* FQFAttributeSet = UFQFBlueprintFunctionLibrary::GetAttributeSet(this);	
	if (FQFAttributeSet == nullptr) return false;

	//if GameplayEffect class is Fluff
	if (EffectedAttribute.MatchesTagExact(FFQFGameplayTags::Get().Attributes_Vital_Fluff))
	{
		const float ReservedFluff = FQFAttributeSet->GetFluff() - FQFAttributeSet->GetLoadedFluff();
		if (ReservedFluff >= 100.f)
		{
			// TELL PLAYER RESERVE FLUFF FULL
			NotifyPlayerOfFullAttribute();
			return false;
		}
	}
	
	
	//if GameplayEffect class is Fluff
	if (EffectedAttribute.MatchesTagExact(FFQFGameplayTags::Get().Attributes_Vital_Health))
	{
		if (FQFAttributeSet->GetHealth() == FQFAttributeSet->GetMaxHealth())
		{
			//TELL PLAYER HEALTH FULL
			NotifyPlayerOfFullAttribute();
			return false;
		}
	}
	 //Apply Effect
	
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	return  true;
	

	
}

void AEffectActor::NotifyPlayerOfFullAttribute()
{
	if (UOverlayWidgetController* OverlayWidgetController = UFQFBlueprintFunctionLibrary::GetOverlayWidgetController(this))
	{
		const FUIWidgetRow* Row = OverlayWidgetController->GetDataTableRowByTag<FUIWidgetRow>(OverlayWidgetController->MessageWidgetDataTable, ErrorInPickupTag);
		OverlayWidgetController->MessageWidgetRowDelegate.Broadcast(*Row);
	}	
}

