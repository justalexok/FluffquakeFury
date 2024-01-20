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

public:

	APippaCharacter();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//CombatInterface
	virtual int32 GetPlayerLevel() override;
	virtual void KnockbackCharacter(float Pitch, float Magnitude) override;
private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, Category = "Abilities") 
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

protected:
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	float TargetArmLength = 750.f;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmAngle = -35.f;	
	
	void AddCharacterAbilities();

};
