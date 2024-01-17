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

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> DynamicImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ExplosionEffect;

	UPROPERTY(EditAnywhere)
	FGameplayTag PillowType;

	UPROPERTY(EditAnywhere)
	FScalableFloat SpawnRate;
	
protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> PillowMesh;

private:


};
