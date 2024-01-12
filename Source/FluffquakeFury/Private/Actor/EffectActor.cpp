// Copyright Alex Goulder

#include "Actor/EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "UI/OverlayWidgetController.h"
#include "Components/BoxComponent.h" 

// Sets default values
AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	
	BlockingVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockingVolume"));
	RootComponent = BlockingVolume;

	BlockingVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BlockingVolume->SetCollisionObjectType(ECC_WorldStatic);
	BlockingVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

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
			EnableBlockingVolume(true);
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
			EnableBlockingVolume(true);
			NotifyPlayerOfFullAttribute();
			return false;
		}
	}
	 //Apply Effect
	EnableBlockingVolume(false);

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
	return  true;
	
}

void AEffectActor::NotifyPlayerOfFullAttribute() const
{
	if (UOverlayWidgetController* OverlayWidgetController = UFQFBlueprintFunctionLibrary::GetOverlayWidgetController(this))
	{
		const FUIWidgetRow* Row = OverlayWidgetController->GetDataTableRowByTag<FUIWidgetRow>(OverlayWidgetController->MessageWidgetDataTable, ErrorInPickupTag);
		OverlayWidgetController->MessageWidgetRowDelegate.Broadcast(*Row);
	}	
}

void AEffectActor::EnableBlockingVolume(const bool bEnabled) const
{
	if (bEnabled)
	{
		BlockingVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		BlockingVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
	else
	{
		BlockingVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		BlockingVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

}

