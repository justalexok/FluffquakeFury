// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/FQFCharacterBase.h"
#include "PippaCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API APippaCharacter : public AFQFCharacterBase
{
	GENERATED_BODY()

	APippaCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
private:
	void InitAbilityActorInfo();

protected:
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	float TargetArmLength = 750.f;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmAngle = -45.f;	
	
};
