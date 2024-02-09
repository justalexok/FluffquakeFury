// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Character/FQFCharacterBase.h"
#include "PippaCharacter.generated.h"

class APippaPlayerController;
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
	virtual void KnockbackCharacter_Implementation(float Magnitude, float Pitch, FVector Direction) override;
	virtual void Die() override;

	UFUNCTION()
	void PippaHandleLevelFailure();

	virtual void Landed(const FHitResult& Hit) override;
private:
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, Category = "Abilities") 
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	APippaPlayerController* PippaPlayerController;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly)
	float TargetArmLength = 750.f;

	UPROPERTY(EditDefaultsOnly)
	float SpringArmAngle = -35.f;
	
	void AddCharacterAbilities();

};
