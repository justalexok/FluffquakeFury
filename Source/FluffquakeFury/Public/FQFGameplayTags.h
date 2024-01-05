// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * FQFGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FFQFGameplayTags
{
public:
	static const FFQFGameplayTags& Get() { return GameplayTags;}
	static void InitializeNativeGameplayTags();

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_MaxHealth;
	FGameplayTag Attributes_Primary_MaxFluff;
	FGameplayTag Attributes_Primary_LoadedFluff;


	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
protected:

private:
	static FFQFGameplayTags GameplayTags;
};