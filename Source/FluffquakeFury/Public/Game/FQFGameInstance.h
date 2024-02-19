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

	//Level
	UPROPERTY(EditAnywhere)
	int32 CurrentLevel = 1;

	UFUNCTION(BlueprintCallable)
	void SetCurrentLevel(const int32 InLevel) {CurrentLevel = InLevel;}

	UFUNCTION(BlueprintCallable)
	void AddToLevel(int32 InLevel) {CurrentLevel += InLevel;}

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentLevel() const { return CurrentLevel;}


	//World
	UPROPERTY(EditAnywhere)
	int32 CurrentWorldIndex = 1;

	UFUNCTION(BlueprintCallable)
	void SetCurrentWorldIndex(const int32 InWorldIndex) {CurrentWorldIndex = InWorldIndex;}

	UFUNCTION(BlueprintCallable)
	void AddToWorldIndex(int32 InWorldIndex) {CurrentWorldIndex += InWorldIndex;}

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentWorldIndex() const { return CurrentWorldIndex;}
	
};
