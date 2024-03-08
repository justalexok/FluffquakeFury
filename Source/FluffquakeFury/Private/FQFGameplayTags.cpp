// Copyright Alex Goulder


#include "FQFGameplayTags.h"
#include "GameplayTagsManager.h"


FFQFGameplayTags FFQFGameplayTags::GameplayTags;

void FFQFGameplayTags::InitializeNativeGameplayTags(){

	GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"),FString("Amount of Health"));
	GameplayTags.Attributes_Vital_Fluff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Fluff"),FString("Amount of Fluff in total stock"));

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Boosts defence against attacks"));
	GameplayTags.Attributes_Primary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxHealth"),FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Primary_MaxFluff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxFluff"),FString("Maximum amount of Fluff obtainable"));
	GameplayTags.Attributes_Primary_LoadedFluff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.LoadedFluff"),FString("Amount of Fluff loaded into Pillow"));
	GameplayTags.Attributes_Primary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.BlockChance"),FString("Chances of blocking an attack"));

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"),FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"),FString("Input Tag for Right Mouse Button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"),FString("Input Tag for 1 key"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"),FString("Input Tag for 2 key"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"),FString("Input Tag for 3 key"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"),FString("Input Tag for 4 key"));

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),FString("Tag for Damage"));
	GameplayTags.DamageType_Fluff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Fluff"),FString("Fluff Damage Type Tag"));
	GameplayTags.DamageType_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Physical"),FString("Physical Damage Type Tag"));
	GameplayTags.DamageType_Insect = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Insect"),FString("Insect Damage Type Tag"));
	GameplayTags.DamageType_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("DamageType.Poison"),FString("Poison Damage Type Tag"));

	GameplayTags.ExplosionChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ExplosionChance"),FString("Tag for Explosion Chance"));

	//Pillow Types
	GameplayTags.PillowType_Regular = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("PillowType_Regular"),FString("Tag for the regular Pillow"));
	GameplayTags.PillowType_Poison = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("PillowType_Poison"),FString("Tag for the Poison Pillow"));

	//Combat Sockets
	GameplayTags.CombatSocket_CenterChest = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.CenterChest"),FString("Socket Tag for the Center of the Chest"));

	
	//Ability Tags
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"),FString("Attack Ability Tag"));
	GameplayTags.Abilities_Jump = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Jump"),FString("Pippa's Jump Ability Tag"));
	GameplayTags.Abilities_CastSpell = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.CastSpell"),FString("Cast Spell Ability Tag"));
	GameplayTags.Abilities_Teleport = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Teleport"),FString("Teleport Ability Tag"));
	GameplayTags.Abilities_PillowWhack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.PillowWhack"),FString("Pillow Whack Ability Tag"));
	GameplayTags.Abilities_PillowSpin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.PillowSpin"),FString("Pillow Spin Ability Tag"));
	GameplayTags.Abilities_TwoHandedJumpAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.TwoHandedJumpAttack"),FString("Two Handed Jump Attack Ability Tag"));
	GameplayTags.Abilities_Squash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Squash"),FString("Squash Attack Ability Tag"));
	GameplayTags.Abilities_TongueAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.TongueAttack"),FString("TongueAttack Ability Tag"));

	//CooldownTags
	GameplayTags.Cooldown_PillowWhack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.PillowWhack"),FString("Pillow Whack Cooldown Tag"));
	GameplayTags.Cooldown_PillowSpin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.PillowSpin"),FString("Pillow Spin Cooldown Tag"));

	//Effect Tags
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),FString("Tag granted when Hit Reacting"));
	GameplayTags.Effects_KnockOver = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.KnockOver"),FString("Tag granted when Knocking Over"));

	//Montage Tags
	GameplayTags.Montage_Attack_PillowWhack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.PillowWhack"),FString("Tag for PillowWhack Attack"));
	GameplayTags.Montage_Attack_PillowSpin = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.PillowSpin"),FString("Tag for PillowSpin Attack"));
	GameplayTags.Montage_Attack_TwoHandedJump = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Jump"),FString("Tag for Jump Attack"));
	GameplayTags.Montage_Attack_CastSpell = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.CastSpell"),FString("Tag for Cast Spell Attack"));
	GameplayTags.Montage_Attack_Squash = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Squash"),FString("Tag for Squash Attack"));
	GameplayTags.Montage_Attack_Tongue = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.Tongue"),FString("Tag for Tongue Attack"));
	GameplayTags.Montage_Attack_FrisbeeThrow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.FrisbeeThrow"),FString("Tag for FrisbeeThrow Attack"));

	//Add to Damage Types TArray
	GameplayTags.DamageTypes.Add(GameplayTags.DamageType_Fluff);
	GameplayTags.DamageTypes.Add(GameplayTags.DamageType_Physical);
	GameplayTags.DamageTypes.Add(GameplayTags.DamageType_Insect);
	GameplayTags.DamageTypes.Add(GameplayTags.DamageType_Poison);
}
