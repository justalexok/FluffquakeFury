// Copyright Alex Goulder


#include "Character/Enemy/EnemyBase.h"

#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "FluffquakeFury/FluffquakeFury.h"
#include "UI/FQFUserWidget.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "AI/FQFAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UFQFAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UFQFAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());

	BaseWalkSpeed = 250.f;
}

void AEnemyBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	FQFAIController = Cast<AFQFAIController>(NewController);
	FQFAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	FQFAIController->RunBehaviorTree(BehaviorTree);
	FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AEnemyBase::Die()
{
	SetLifeSpan(5.f);
	Super::Die();
}

void AEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UE_LOG(LogTemp,Warning,TEXT("CharacterMovement MaxWalkSpeed: %f"), GetCharacterMovement()->MaxWalkSpeed);
	UE_LOG(LogTemp,Warning,TEXT("BaseWalkSpeed: %f"), BaseWalkSpeed);

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();
	UFQFBlueprintFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent);


	if (UFQFUserWidget* FQFUserWidget = Cast<UFQFUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		FQFUserWidget->SetWidgetController(this);
	}

	if (const UFQFAttributeSet* FQFAS = Cast<UFQFAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FQFAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FQFAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->RegisterGameplayTagEvent(FFQFGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AEnemyBase::HitReactTagChanged

		);

		OnHealthChanged.Broadcast(FQFAS->GetHealth());
		OnMaxHealthChanged.Broadcast(FQFAS->GetMaxHealth());
	}
	
}

void AEnemyBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

void AEnemyBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UFQFAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();

}

void AEnemyBase::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyBase::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AEnemyBase::GetPlayerLevel()
{
	return Level;
}
