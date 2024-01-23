// Copyright Alex Goulder


#include "Player/PippaPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "FQFGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "Actor/EffectActor.h"
#include "Character/FQFCharacterBase.h"
#include "Character/Pippa/PippaCharacter.h"
#include "Components/SplineComponent.h"
#include "Input/FQFInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "UI/DamageTextComponent.h"


APippaPlayerController::APippaPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

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
	FQFInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &APippaPlayerController::ShiftPressed);
	FQFInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &APippaPlayerController::ShiftReleased);
	FQFInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APippaPlayerController::Jump);
	FQFInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void APippaPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
	AutoRun();
}

void APippaPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bPillowExploded, bool bFatalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, TargetCharacter, bBlockedHit, bPillowExploded, bFatalHit);
	}
}


void APippaPlayerController::ShowPickupWidget_Implementation(float ChangeAmount,
	FGameplayTag AttributeTag, AEffectActor* EffectActor)
{
	if (IsValid(EffectActor) && PickupTextComponentClass)
	{
		UPickupTextComponent* HealthChangeText = NewObject<UPickupTextComponent>(EffectActor, PickupTextComponentClass);
		HealthChangeText->RegisterComponent();
		HealthChangeText->AttachToComponent(EffectActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		HealthChangeText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		HealthChangeText->SetPickupText(ChangeAmount, AttributeTag);		
	}
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

void APippaPlayerController::Jump(const FInputActionValue& InputActionValue)
{
	if (APippaCharacter* PippaCharacter = Cast<APippaCharacter>(GetPawn<APawn>()))
	{
		PippaCharacter->Jump();
		
	}

}

void APippaPlayerController::CursorTrace()
{
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
	if (InputTag.MatchesTagExact(FFQFGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}

}



void APippaPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//Not LMB
	if (!InputTag.MatchesTagExact(FFQFGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		
		return;
	}
	//LMB and Targeting
	if (bTargeting || bShiftKeyDown) 
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	}
	//LMB and not targeting. 
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;


		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}

}

void APippaPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FFQFGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	if (!bTargeting && !bShiftKeyDown)
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			UE_LOG(LogTemp, Display, TEXT("CachedDestination: %s"), *CachedDestination.ToString());
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num() >0 )
				{
					CachedDestination = NavPath->PathPoints.Last();
					//Sets off in Tick
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

UFQFAbilitySystemComponent* APippaPlayerController::GetASC()
{
	if (FQFAbilitySystemComponent == nullptr)
	{
		FQFAbilitySystemComponent = Cast<UFQFAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return FQFAbilitySystemComponent;
}

void APippaPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}
