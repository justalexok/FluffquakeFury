// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PillowProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class FLUFFQUAKEFURY_API APillowProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	APillowProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	float ProjectileGravityScale = 1;

protected:
	virtual void BeginPlay() override;	

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;


};
