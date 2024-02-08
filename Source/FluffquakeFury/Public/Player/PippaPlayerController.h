// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Game/FQFGameModeBase.h"
#include "UI/PickupTextComponent.h"
#include "PippaPlayerController.generated.h"

class AEffectActor;
class UDamageTextComponent;
struct FGameplayTag;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UFQFAbilitySystemComponent;
class UFQFInputConfig;
class USplineComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelFailed);

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API APippaPlayerController : public APlayerController
{
	GENERATED_BODY()

	APippaPlayerController();

public:
	
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bPillowExploded, bool bFatalHit);

	UFUNCTION(Client, Reliable)
	void ShowPickupWidget(float ChangeAmount, FGameplayTag AttributeTag, AEffectActor* EffectActor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LevelSecondsRemaining;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnLevelFailed OnLevelFailureDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bLevelIsRunning = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PippaContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> JumpAction;
	
	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;
	
	void Move(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);


	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	
	UPROPERTY()
	TObjectPtr<UFQFAbilitySystemComponent> FQFAbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	
	TObjectPtr<UFQFInputConfig> InputConfig;
	UFQFAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	// void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPickupTextComponent> PickupTextComponentClass;

	UPROPERTY()
	AFQFGameModeBase* GameMode;

	UPROPERTY()
	bool HasCheckedLevelFinished = false;
};
