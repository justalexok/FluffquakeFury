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
#include "Player/FQFPlayerState.h"
#include "Player/PippaPlayerController.h"

// Sets default values
AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
	
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

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootStaticMesh);	
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();	


}

bool AEffectActor::IsAttributeFull(FGameplayAttribute Attribute)
{
	const AFQFPlayerState* PlayerState = UGameplayStatics::GetPlayerController(this,0)->GetPlayerState<AFQFPlayerState>();
	if (PlayerState == nullptr) return false;

	const UFQFAttributeSet* AttributeSet = Cast<UFQFAttributeSet>(PlayerState->GetAttributeSet());
	if (AttributeSet == nullptr) return false;

	const UAbilitySystemComponent* PippaASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PippaASC == nullptr) return false;

	if (Attribute == AttributeSet->GetFluffAttribute())
	{
		return AttributeSet->GetFluff() == AttributeSet->GetMaxFluff();
	}
	if (Attribute == AttributeSet->GetHealthAttribute())
	{
		return AttributeSet->GetHealth() == AttributeSet->GetMaxHealth();
	}
	return false;
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
	

	const float ModifiedMagnitude = GetModifiedMagnitude(GameplayEffectClass);

	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	if (!bIsDamageCauser) ShowPickupText(ModifiedMagnitude, Target,EffectedAttribute);

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

void AEffectActor::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		//Loop over each Material in the SKM and swap for DynamicMatInst
		for (int32 i = 0; i < RootStaticMesh->GetMaterials().Num(); ++i)
		{
			RootStaticMesh->SetMaterial(i, DynamicMatInst);
		}	
		StartDissolveTimeline(DynamicMatInst);
	}
}


