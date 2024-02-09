// Copyright Alex Goulder


#include "Character/Pippa/PippaCharacter.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "AI/FQFAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/Enemy/EnemyBase.h"
#include "Player/PippaPlayerController.h"
#include "UI/HUD/FQFHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FQFPlayerState.h"


APippaCharacter::APippaCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = TargetArmLength;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	SpringArm->SetRelativeRotation(FRotator(SpringArmAngle, 0.0f, 0.0f));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	 

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	if (AFQFPlayerState* PS = Cast<AFQFPlayerState>(GetPlayerState()))
	{
		AbilitySystemComponent = PS->GetAbilitySystemComponent();
		AttributeSet = PS->GetAttributeSet();
	}
	
}

void APippaCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(APippaPlayerController* PC = Cast<APippaPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->OnLevelFailureDelegate.AddDynamic(this,&APippaCharacter::PippaHandleLevelFailure);
	}
	UFQFBlueprintFunctionLibrary::GetFQFGameMode(this)->OnLevelCompletionDelegate.AddDynamic(this,&APippaCharacter::PippaHandleLevelUp);
}

void APippaCharacter::AddCharacterAbilities()
{
	UFQFAbilitySystemComponent* FQFASC = CastChecked<UFQFAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	FQFASC->AddCharacterAbilities(StartupAbilities);
}


void APippaCharacter::InitAbilityActorInfo()
{
	AFQFPlayerState* FQFPlayerState = GetPlayerState<AFQFPlayerState>();
	check(FQFPlayerState);
	FQFPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FQFPlayerState, this);
	AbilitySystemComponent = FQFPlayerState->GetAbilitySystemComponent();
	Cast<UFQFAbilitySystemComponent>(FQFPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AttributeSet = FQFPlayerState->GetAttributeSet();

	PippaPlayerController = Cast<APippaPlayerController>(GetController());
	if (PippaPlayerController)
	{
		if (AFQFHUD* FQFHUD = Cast<AFQFHUD>(PippaPlayerController->GetHUD()))
		{
			FQFHUD->InitOverlay(PippaPlayerController, FQFPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();

	if (PillowClass)
	{
		Pillow = GetWorld()->SpawnActor<APillowBase>(PillowClass, FVector::ZeroVector, FRotator::ZeroRotator);

	}
	
	
}


void APippaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void APippaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 APippaCharacter::GetPlayerLevel()
{	
	const AFQFPlayerState* FQFPlayerState = GetPlayerState<AFQFPlayerState>();
	check(FQFPlayerState);
	return FQFPlayerState->GetPlayerLevel();
}

void APippaCharacter::KnockbackCharacter_Implementation(float Magnitude, float Pitch, FVector Direction)
{
	// const FVector OppositeDirection = GetActorForwardVector() * -1.0f;
	FRotator Rotation = Direction.Rotation();
	Rotation.Pitch = Pitch;
	const FVector ToTarget = Rotation.Vector();
	const FVector KnockbackForce = ToTarget * Magnitude;
	LaunchCharacter(KnockbackForce, true, true);
}

void APippaCharacter::Die()
{
	Super::Die();

	//Tell each enemy behavior tree player is Dead.
	TArray<AActor*> EnemiesInWorld;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyBase::StaticClass(), EnemiesInWorld);

	for (AActor* EnemyInWorld : EnemiesInWorld)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(EnemyInWorld);
		if (Enemy)
		{
			Enemy->FQFAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsPlayerDead"), true);
			Enemy->RemoveInfiniteGameplayEffects();
		}
	}
}

void APippaCharacter::PippaHandleLevelFailure()
{
	if (PippaPlayerController)
	{
		PippaPlayerController->DisableInput(PippaPlayerController);
	}
}

void APippaCharacter::PippaHandleLevelUp()
{
	AFQFPlayerState* FQFPlayerState = GetPlayerState<AFQFPlayerState>();
	check(FQFPlayerState);
	FQFPlayerState->AddToLevel(1);
}

void APippaCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	//Activate Jump Ability
	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(FFQFGameplayTags::Get().Abilities_Jump);
	GetAbilitySystemComponent()->TryActivateAbilitiesByTag(TagContainer);

}

