// Copyright Alex Goulder


#include "FQFAssetManager.h"
#include "FQFGameplayTags.h"

UFQFAssetManager& UFQFAssetManager::Get()
{
	check(GEngine);

	UFQFAssetManager* AuraAssetManager = Cast<UFQFAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UFQFAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FFQFGameplayTags::InitializeNativeGameplayTags();
}
