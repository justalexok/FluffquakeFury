// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

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

	void EnableBlockingVolume(bool bEnabled) const;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BlockingVolume;

};
