// Copyright Alex Goulder

#include "Actor/EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FQFPlayerState.h"

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

void AEffectActor::ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC == nullptr) return;

	UFQFAttributeSet* FQFAttributeSet = UFQFBlueprintFunctionLibrary::GetAttributeSet(this);	
	if (FQFAttributeSet == nullptr) return;	
	
	const float ReservedFluff = FQFAttributeSet->GetFluff() - FQFAttributeSet->GetLoadedFluff();
	if (ReservedFluff >= 100.f)
	{
		//TODO TELL PLAYER RESERVE FLUFF IS TOO FULL
	}
	else
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	}
	
	

	
}

