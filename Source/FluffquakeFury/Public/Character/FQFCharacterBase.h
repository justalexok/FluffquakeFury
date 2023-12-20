// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FQFCharacterBase.generated.h"

UCLASS()
class FLUFFQUAKEFURY_API AFQFCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AFQFCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
