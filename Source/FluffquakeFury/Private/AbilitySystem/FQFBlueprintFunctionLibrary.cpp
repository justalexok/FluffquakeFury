// Copyright Alex Goulder


#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"

#include "AbilitySystem/FQFAttributeSet.h"
#include "Game/FQFGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FQFPlayerState.h"
#include "UI/FQFWidgetController.h"
#include "UI/HUD/FQFHUD.h"
#include "FQFAbilityTypes.h"



UOverlayWidgetController* UFQFBlueprintFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFQFHUD* FQFHUD = Cast<AFQFHUD>(PC->GetHUD()))
		{
			AFQFPlayerState* PS = PC->GetPlayerState<AFQFPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			
			return FQFHUD->GetOverlayWidgetController(WidgetControllerParams);
			
		}
	}
	return nullptr;
}

void UFQFBlueprintFunctionLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AFQFGameModeBase* FQFGameMode = Cast<AFQFGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FQFGameMode == nullptr) return;
	
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = FQFGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

}

void UFQFBlueprintFunctionLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	AFQFGameModeBase* FQFGameMode = Cast<AFQFGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (FQFGameMode == nullptr) return;

	UCharacterClassInfo* CharacterClassInfo = FQFGameMode->CharacterClassInfo;
	if (CharacterClassInfo == nullptr) return;

	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
	}
}


void UFQFBlueprintFunctionLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
                                                              TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
                                                              const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

UFQFAttributeSet* UFQFBlueprintFunctionLibrary::GetAttributeSet(const UObject* WorldContextObject)
{
	if (const APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		const AFQFPlayerState* PS = PC->GetPlayerState<AFQFPlayerState>();
		if (UFQFAttributeSet* FQFAS = Cast<UFQFAttributeSet>(PS->GetAttributeSet()))
		{
			return FQFAS;
		}
	}
	return nullptr;
	
}

bool UFQFBlueprintFunctionLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->IsBlockedHit();
	}
	return false;
}

bool UFQFBlueprintFunctionLibrary::HasPillowExploded(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->HasPillowExploded();
	}
	return false;
}

void UFQFBlueprintFunctionLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsBlockedHit)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UFQFBlueprintFunctionLibrary::SetHasPillowExploded(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInHasPillowExploded)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetHasPillowExploded(bInHasPillowExploded);		
	}
}

bool UFQFBlueprintFunctionLibrary::IsNotFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}
