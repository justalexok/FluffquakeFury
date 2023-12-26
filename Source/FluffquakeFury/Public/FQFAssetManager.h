// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FQFAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UFQFAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
	
};
