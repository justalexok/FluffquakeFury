// Copyright Alex Goulder


#include "Player/PippaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Input/FQFInputComponent.h"
#include "Interaction/EnemyInterface.h"


APippaPlayerController::APippaPlayerController()
{
	bReplicates = true;
}

void APippaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(PippaContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(PippaContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void APippaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UFQFInputComponent* FQFInputComponent = CastChecked<UFQFInputComponent>(InputComponent);
	FQFInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APippaPlayerController::Move);
	FQFInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void APippaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void APippaPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void APippaPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor) ThisActor->HighlightActor();

	}
}

void APippaPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());

}

void APippaPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());

}

void APippaPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());

}
