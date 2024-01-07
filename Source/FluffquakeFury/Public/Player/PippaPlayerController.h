// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "PippaPlayerController.generated.h"

struct FGameplayTag;
class IEnemyInterface;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UFQFInputConfig;
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API APippaPlayerController : public APlayerController
{
	GENERATED_BODY()

	APippaPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PippaContext;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);

	void CursorTrace();
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UFQFInputConfig> InputConfig;
	
};
