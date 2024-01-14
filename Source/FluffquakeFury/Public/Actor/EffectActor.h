// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

class UBoxComponent;
struct FGameplayTag;
class UGameplayEffect;

UCLASS()
class FLUFFQUAKEFURY_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	bool ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	FGameplayTag ErrorInPickupTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	FGameplayTag EffectedAttribute;

	void NotifyPlayerOfFullAttribute() const;

	void EnableStaticMeshBlocking(bool bEnabled) const;

	UFUNCTION(BlueprintCallable)
	void RotateActor(float DeltaTime, float Speed);

	UFUNCTION(BlueprintCallable)
	void KnockbackCharacter(AActor* OverlappedActor, float Pitch, float Magnitude);
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RootStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;

};
