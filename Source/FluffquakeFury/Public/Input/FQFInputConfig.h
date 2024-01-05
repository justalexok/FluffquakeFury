// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "FQFInputConfig.generated.h"


USTRUCT(BlueprintType)
struct FFQFInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FFQFInputAction> AbilityInputActions;
	
};
