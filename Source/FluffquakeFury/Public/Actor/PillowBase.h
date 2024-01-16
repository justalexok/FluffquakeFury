// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillowBase.generated.h"

class UNiagaraSystem;

UCLASS()
class FLUFFQUAKEFURY_API APillowBase : public AActor
{
	GENERATED_BODY()

	APillowBase();
	
public:		

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> RegularPillowImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> BlockedPillowImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ExplosionEffect;
	
protected:
	virtual void BeginPlay() override;
	
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> PillowMesh;

private:


};
