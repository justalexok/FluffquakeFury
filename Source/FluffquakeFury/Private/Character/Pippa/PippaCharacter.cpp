// Copyright Alex Goulder


#include "Character/Pippa/PippaCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "Player/PippaPlayerController.h"
#include "UI/HUD/FQFHUD.h"
#include "GameFramework/CharacterMovementComponent.h"
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

void APippaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitAbilityActorInfo();
}

void APippaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void APippaCharacter::InitAbilityActorInfo()
{
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, FString("Init Ability Actor Info in Pippa CHaracter"));

	AFQFPlayerState* FQFPlayerState = GetPlayerState<AFQFPlayerState>();
	check(FQFPlayerState);
	FQFPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(FQFPlayerState, this);
	AbilitySystemComponent = FQFPlayerState->GetAbilitySystemComponent();
	Cast<UFQFAbilitySystemComponent>(FQFPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AttributeSet = FQFPlayerState->GetAttributeSet();

	if (APippaPlayerController* PippaPlayerController = Cast<APippaPlayerController>(GetController()))
	{
		if (AFQFHUD* FQFHUD = Cast<AFQFHUD>(PippaPlayerController->GetHUD()))
		{
			FQFHUD->InitOverlay(PippaPlayerController, FQFPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializePrimaryAttributes();
}
