// Copyright Alex Goulder

#include "Actor/EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/Pippa/PippaCharacter.h"
#include "Components/BoxComponent.h" 
#include "Kismet/GameplayStatics.h"
#include "Player/PippaPlayerController.h"

// Sets default values
AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	
	//Root Mesh = The Box itself. Ignores all. Switch if attribute full following overlap with collision box 
	RootStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Root Static Mesh");
	RootStaticMesh->SetupAttachment(RootComponent);
	RootStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootStaticMesh->SetCollisionObjectType(ECC_WorldStatic);
	RootStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootStaticMesh);	
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	if (bHasTriggerBox)
	{
		TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
		TriggerBox->SetupAttachment(RootComponent);	
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
		TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	}
	else if (TriggerBox) TriggerBox->DestroyComponent();
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

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const float ModifiedMagnitude = GetModifiedMagnitude(GameplayEffectClass);
	

	if (!bIsDamageCauser) ShowPickupText(ModifiedMagnitude, Target,EffectedAttribute);
	

	// if (EffectedAttribute.MatchesTagExact(FFQFGameplayTags::Get().Attributes_Vital_Health))
	// {
	// 	const float ModifiedMagnitude = GetModifiedMagnitude(GameplayEffectClass);
	// 	ShowPickupText(ModifiedMagnitude, Target,FFQFGameplayTags::Get().Attributes_Vital_Health);
	// }
	// if (EffectedAttribute.MatchesTagExact(FFQFGameplayTags::Get().Attributes_Vital_Fluff))
	// {
	// 	ShowPickupText(0.f, Target,FFQFGameplayTags::Get().Attributes_Vital_Fluff);
	// }

	
	return  true;
	
}
void AEffectActor::ShowPickupText(float ChangeAmount, AActor* Target, FGameplayTag AttributeTag) 
{

	if (ACharacter* TargetCharacter = Cast<ACharacter>(Target))
	{
		if(APippaPlayerController* PC = Cast<APippaPlayerController>(UGameplayStatics::GetPlayerController(TargetCharacter, 0)))
		{
			PC->ShowPickupWidget(ChangeAmount, AttributeTag,this);
		}
	}
}
void AEffectActor::RotateActor(float DeltaTime, float Speed)
{
	FRotator NewRotation = GetActorRotation() + FRotator(0.0f, Speed * DeltaTime, 0.0f);
	SetActorRotation(NewRotation);
}

float AEffectActor::GetModifiedMagnitude(TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	float ModifiedMagnitude = 0.f;	
	if (UGameplayEffect* GameplayEffect = GameplayEffectClass.GetDefaultObject())
	for (FGameplayModifierInfo Mod: GameplayEffect->Modifiers)
	{
		Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(1,ModifiedMagnitude);
	}
	return  ModifiedMagnitude;
}

//

// void AEffectActor::NotifyPlayerOfFullAttribute() const
// {
// 	if (UOverlayWidgetController* OverlayWidgetController = UFQFBlueprintFunctionLibrary::GetOverlayWidgetController(this))
// 	{
// 		const FUIWidgetRow* Row = OverlayWidgetController->GetDataTableRowByTag<FUIWidgetRow>(OverlayWidgetController->MessageWidgetDataTable, ErrorInPickupTag);
// 		OverlayWidgetController->MessageWidgetRowDelegate.Broadcast(*Row);
// 	}	
// }
//
// void AEffectActor::EnableStaticMeshBlocking(const bool bEnabled) const
// {
// 	if (bEnabled)
// 	{
// 		RootStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
// 		RootStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
// 	}
// 	else
// 	{
// 		RootStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 		RootStaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
// 	}
//
// }


// 		



