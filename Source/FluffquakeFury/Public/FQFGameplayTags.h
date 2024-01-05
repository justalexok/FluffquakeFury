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
protected:

private:
	static FFQFGameplayTags GameplayTags;
};