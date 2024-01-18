// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "PillowBase.generated.h"

struct FScalableFloat;
class UNiagaraSystem;

UCLASS()
class FLUFFQUAKEFURY_API APillowBase : public AActor
{
	GENERATED_BODY()

	APillowBase();
	
public:

	// UPROPERTY(EditAnywhere)
	// TObjectPtr<USkeletalMeshComponent> TestPillowMesh;

	UPROPERTY(EditAnywhere, Category="Pillow Properties")
	TObjectPtr<UNiagaraSystem> DynamicImpactEffect;

	UPROPERTY(EditAnywhere, Category="Pillow Properties")
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditAnywhere, Category="Pillow Properties")
	FGameplayTag PillowType;

	UPROPERTY(EditAnywhere, Category="Pillow Properties")
	FScalableFloat SpawnRate;

	UPROPERTY(EditAnywhere, Category="Pillow Properties")
	FLinearColor FluffColor;
	
protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> PillowMesh;

private:


};
