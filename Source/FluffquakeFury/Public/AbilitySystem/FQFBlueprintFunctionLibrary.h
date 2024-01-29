// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "FQFBlueprintFunctionLibrary.generated.h"

struct FDamageEffectParams;
class UFQFAttributeSet;
class UAbilitySystemComponent;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category="FQFBlueprintFunctionLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="FQFBlueprintFunctionLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="FQFBlueprintFunctionLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category = "FQFBlueprintFunctionLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "FQFBlueprintFunctionLibrary|GameplayMechanics")
	static UFQFAttributeSet* GetAttributeSet(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "FQFBlueprintFunctionLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "FQFBlueprintFunctionLibrary|GameplayEffects")
	static bool HasPillowExploded(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "FQFBlueprintFunctionLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "FQFBlueprintFunctionLibrary|GameplayEffects")
	static void SetHasPillowExploded(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInHasPillowExploded);

	UFUNCTION(BlueprintPure, Category = "FQFBlueprintFunctionLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	UFUNCTION(BlueprintPure, Category = "FQFBlueprintFunctionLibrary|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);

	static bool IsTargetImmuneToDamageType(AActor* TargetActor, FGameplayTag DamageType);

	UFUNCTION(BlueprintCallable, Category = "FQFBlueprintFunctionLibrary|GameplayEffects")
	static void SetRecentlyReceivedDamageTag(AActor* TargetActor, const FGameplayTag DamageType);
	
	UFUNCTION(BlueprintCallable, Category = "FQFBlueprintFunctionLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	
};
