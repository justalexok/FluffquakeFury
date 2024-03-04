// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/FQFAttributeSet.h"
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
	FGameplayTag EffectedAttribute;

	void ShowPickupText(float ChangeAmount, AActor* Target, FGameplayTag AttributeTag);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	UStaticMeshComponent* RootStaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	bool bHasTriggerBox = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	bool bIsDamageCauser = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	bool bShouldSpin = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float DelayBeforeDestroy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	TObjectPtr<UAnimMontage> ImpactAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	TObjectPtr<UAnimMontage> DestroyAnim;
	
	UFUNCTION(BlueprintCallable)
	void RotateActor(float DeltaTime, float Speed);

	float GetModifiedMagnitude(TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UFUNCTION(BlueprintCallable)
	bool IsAttributeFull(FGameplayAttribute Attribute);
};
