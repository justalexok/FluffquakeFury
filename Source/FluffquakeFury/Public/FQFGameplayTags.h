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
	FGameplayTag DamageType_Poison;

	FGameplayTag ExplosionChance;

	FGameplayTag PillowType_Regular;
	FGameplayTag PillowType_Poison;

	FGameplayTag Abilities_Attack; //General attack tag used by enemy's BehaviourTree - they each only have one ability
	FGameplayTag Abilities_Jump; //Pippa's jump
	FGameplayTag Abilities_CastSpell; //Unused
	FGameplayTag Abilities_Teleport; //Activated directly using this tag in HitReact (if Jolleen)
	FGameplayTag Abilities_PillowWhack; //Used in AbilityInfo DataAsset
	FGameplayTag Abilities_PillowSpin; //Used in AbilityInfo DataAsset
	FGameplayTag Abilities_TwoHandedJumpAttack; //Used in AbilityInfo DataAsset
	FGameplayTag Abilities_Squash; //Used in AbilityInfo DataAsset
	FGameplayTag Abilities_TongueAttack; //Used in AbilityInfo DataAsset


	FGameplayTag Cooldown_PillowWhack;
	FGameplayTag Cooldown_PillowSpin;

	FGameplayTag Effects_HitReact;
	FGameplayTag Effects_KnockOver;

	FGameplayTag CombatSocket_CenterChest;

	FGameplayTag Montage_Attack_PillowWhack;
	FGameplayTag Montage_Attack_PillowSpin;
	FGameplayTag Montage_Attack_TwoHandedJump;
	FGameplayTag Montage_Attack_CastSpell;
	FGameplayTag Montage_Attack_Squash;
	FGameplayTag Montage_Attack_Tongue;
	FGameplayTag Montage_Attack_FrisbeeThrow;


	TArray<FGameplayTag> DamageTypes;
protected:

private:
	static FFQFGameplayTags GameplayTags;
};