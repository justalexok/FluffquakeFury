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
	
	//Root Mesh = The Box itself. Ignores all. Switch if attribute full following overlap with collision box 
	RootStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Root Static Mesh");
	RootStaticMesh->SetupAttachment(RootComponent);
	RootStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootStaticMesh->SetCollisionObjectType(ECC_WorldStatic);
	RootStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);	
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
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
			EnableStaticMeshBlocking(true);
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
			EnableStaticMeshBlocking(true);
			NotifyPlayerOfFullAttribute();
			return false;
		}
	}
	 //Apply Effect
	EnableStaticMeshBlocking(false);

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

void AEffectActor::EnableStaticMeshBlocking(const bool bEnabled) const
{
	if (bEnabled)
	{
		RootStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		RootStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	}
	else
	{
		RootStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RootStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}

}

void AEffectActor::RotateActor(float DeltaTime, float Speed)
{
	FRotator NewRotation = GetActorRotation() + FRotator(0.0f, Speed * DeltaTime, 0.0f);
	SetActorRotation(NewRotation);
}

