// Copyright Alex Goulder


#include "FQFGameplayTags.h"
#include "GameplayTagsManager.h"


FFQFGameplayTags FFQFGameplayTags::GameplayTags;

void FFQFGameplayTags::InitializeNativeGameplayTags(){

	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Increases physical damage"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Boosts defence against attacks"));
	GameplayTags.Attributes_Primary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxHealth"),FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Primary_MaxFluff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.MaxFluff"),FString("Maximum amount of Fluff obtainable"));
}
