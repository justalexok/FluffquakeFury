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

	FGameplayTag Attributes_Vital_Health;
	FGameplayTag Attributes_Vital_Fluff;

	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_MaxHealth;
	FGameplayTag Attributes_Primary_MaxFluff;
	FGameplayTag Attributes_Primary_LoadedFluff;
	FGameplayTag Attributes_Primary_BlockChance;


	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	//DamageTypes
	FGameplayTag Damage;
	FGameplayTag DamageType_Fluff;
	FGameplayTag DamageType_Physical;
	FGameplayTag DamageType_Insect;

	FGameplayTag ExplosionChance;

	FGameplayTag PillowType_Regular;
	FGameplayTag PillowType_Poison;

	FGameplayTag Abilities_Attack;

	FGameplayTag Effects_HitReact;

	FGameplayTag CombatSocket_CenterChest;

	FGameplayTag Montage_Attack_PillowWhack;
	FGameplayTag Montage_Attack_Jump;
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	TArray<FGameplayTag> DamageTypes;
protected:

private:
	static FFQFGameplayTags GameplayTags;
};