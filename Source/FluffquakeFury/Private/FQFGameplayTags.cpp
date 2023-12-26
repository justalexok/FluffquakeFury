// Copyright Alex Goulder


#include "FQFGameplayTags.h"
#include "GameplayTagsManager.h"


FFQFGameplayTags FFQFGameplayTags::GameplayTags;

void FFQFGameplayTags::InitializeNativeGameplayTags(){

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Maximum amount of Strength obtainable"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Maximum amount of Resilience obtainable"));
	GameplayTags.Attributes_Primary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxHealth"),FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Primary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxMana"),FString("Maximum amount of Mana obtainable"));
}
