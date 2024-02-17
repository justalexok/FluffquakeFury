// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "FQFGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 CurrentLevel = 1;

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevel(const int32 InLevel) {CurrentLevel = InLevel;}

	UFUNCTION(BlueprintCallable)
	void AddToLevel(int32 InLevel) {CurrentLevel += InLevel;}

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentLevel() const { return CurrentLevel;}
	
};
