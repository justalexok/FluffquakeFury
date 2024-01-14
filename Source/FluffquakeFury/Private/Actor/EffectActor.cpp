// Copyright Alex Goulder

#include "Actor/EffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/Pippa/PippaCharacter.h"
#include "UI/OverlayWidgetController.h"
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
	
	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	if (EffectedAttribute.MatchesTagExact(FFQFGameplayTags::Get().Attributes_Vital_Health))
	{

		ShowPickupText(91.f, Target,FFQFGameplayTags::Get().Attributes_Vital_Health);
	}
	if (EffectedAttribute.MatchesTagExact(FFQFGameplayTags::Get().Attributes_Vital_Fluff))
	{
		ShowPickupText(0.f, Target,FFQFGameplayTags::Get().Attributes_Vital_Fluff);
	}	
	
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

void AEffectActor::ShowPickupText(float ChangeAmount, AActor* Target, FGameplayTag AttributeTag) const
{

	if (ACharacter* TargetCharacter = Cast<ACharacter>(Target))
	{
		if(APippaPlayerController* PC = Cast<APippaPlayerController>(UGameplayStatics::GetPlayerController(TargetCharacter, 0)))
		{
			PC->ShowPickupWidget(ChangeAmount, TargetCharacter, AttributeTag);
		}
	}
}
		
void AEffectActor::RotateActor(float DeltaTime, float Speed)
{
	FRotator NewRotation = GetActorRotation() + FRotator(0.0f, Speed * DeltaTime, 0.0f);
	SetActorRotation(NewRotation);
}

void AEffectActor::KnockbackCharacter(AActor* OverlappedActor, float Pitch, float Magnitude)
{
	if (APippaCharacter* PippaCharacter = Cast<APippaCharacter>(OverlappedActor))
	{
		const FVector Direction = PippaCharacter->GetActorLocation() - GetActorLocation();  
		FRotator Rotation = Direction.Rotation();
		Rotation.Pitch = Pitch;
		const FVector ToTarget = Rotation.Vector();
		const FVector KnockbackForce = ToTarget * Magnitude;
		PippaCharacter->LaunchCharacter(KnockbackForce, true, true);
	}
}

