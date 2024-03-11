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
#include "Kismet/GameplayStatics.h"
#include "Player/PippaPlayerController.h"

class AFQFPlayerState;

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

	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	
	if (CharacterClass == ECharacterClass::Insect)
	{
		GetMesh()->SetCollisionObjectType(ECC_Insect);
	}
}

void AEnemyBase::SetBlackboardBoolForKey_Implementation(FName KeyName, bool Bool)
{
	FQFAIController->GetBlackboardComponent()->SetValueAsBool(KeyName, Bool);
}

bool AEnemyBase::IsDefending_Implementation()
{
	return FQFAIController->GetBlackboardComponent()->GetValueAsBool("AmIDefending");
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if(APippaPlayerController* PC = Cast<APippaPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->OnLevelFailureDelegate.AddDynamic(this,&AEnemyBase::EnemyHandleLevelFailure);
		PC->OnShouldBeginLevelDelegate.AddDynamic(this, &AEnemyBase::EnemyHandleLevelShouldBegin);
	}
	
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();
	UFQFBlueprintFunctionLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);


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

void AEnemyBase::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UFQFAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	// InitializeDefaultAttributes();
	UFQFBlueprintFunctionLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);

}

void AEnemyBase::Die(const FVector& DeathImpulse)
{
	SetLifeSpan(5.f);
	FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsDead"), true);

	PlayAnimMontage(DeathMontage);

	

	Super::Die(DeathImpulse);

	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);


	//Tell GameMode to check if level is complete And Spawn Reward
	if (AFQFGameModeBase* GameMode = UFQFBlueprintFunctionLibrary::GetFQFGameMode(this))
	{
		GameMode->CheckIfLevelComplete();
		if (Level > 1)
		{
			GameMode->SpawnReward();
		}
		//if hotel demo and count for room complete, spawn poison pillow
	}

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

AActor* AEnemyBase::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AEnemyBase::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

void AEnemyBase::EnemyHandleLevelShouldBegin_Implementation()
{
	if (IsDead_Implementation()) return;
	
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(UGameplayStatics::GetPlayerPawn(this,0)))
	{
		int32 PlayerLevel =  CombatInterface->GetPlayerLevel();
		FString EnemyGameLevelTag = FString();
		bool AmSameGameLevelAsPlayer = false;
		for (FName Tag : Tags)
		{
			EnemyGameLevelTag = Tag.ToString();
			FName LevelFName = *FString::FromInt(PlayerLevel);
			if (LevelFName == Tag)
			{
				
				AmSameGameLevelAsPlayer = true;
				break;
			}
		}	
		UE_LOG(LogTemp, Warning, TEXT("I am %s and my Game Level is %s. Player Level is %d"),*GetClass()->GetName(),*EnemyGameLevelTag, PlayerLevel);
		FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsMyLevelRunning"), AmSameGameLevelAsPlayer);

	}
}

FLinearColor AEnemyBase::GetEnemyColor_Implementation()
{
	return CharacterColor;
}

int32 AEnemyBase::GetPlayerLevel()
{
	return Level;
}

void AEnemyBase::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);

	if (CharacterClass == ECharacterClass::Elementalist)
	{
		FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("AmICasting"), false); 
	}
	

}

void AEnemyBase::EnemyHandleLevelFailure()
{
	UE_LOG(LogTemp,Error,TEXT("ENEMY LEVEL FAILURE HANDLED!"))
	FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsLevelRunning"), false); 
}
