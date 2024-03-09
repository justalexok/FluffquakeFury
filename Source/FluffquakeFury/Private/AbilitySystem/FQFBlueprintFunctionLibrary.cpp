// Copyright Alex Goulder


#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "Game/FQFGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FQFPlayerState.h"
#include "UI/FQFWidgetController.h"
#include "UI/HUD/FQFHUD.h"
#include "FQFAbilityTypes.h"
#include "FQFGameplayTags.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/Enemy/EnemyBase.h"
#include "Game/FQFGameInstance.h"

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
	if (const AEnemyBase* Enemy = Cast<AEnemyBase>(WorldContextObject))
	{
		return Cast<UFQFAttributeSet>(Enemy->AttributeSet);
	}
	
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

FGameplayTag UFQFBlueprintFunctionLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (FQFEffectContext->GetDamageType().IsValid())
		{
			return *FQFEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

void UFQFBlueprintFunctionLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		FQFEffectContext->SetDamageType(DamageType);
	}
}

bool UFQFBlueprintFunctionLibrary::IsNotFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

bool UFQFBlueprintFunctionLibrary::IsDead(const AActor* Actor)
{
	return Cast<AFQFCharacterBase>(Actor)->IsDead_Implementation();
}

bool UFQFBlueprintFunctionLibrary::IsTargetImmuneToDamageType(AActor* TargetActor, const FGameplayTag DamageType) 
{
	if (const AEnemyBase* Enemy = Cast<AEnemyBase>(TargetActor))
	{
		if (Enemy->Immunities.Contains(DamageType)) return true;			
	}
	return false;
}

void UFQFBlueprintFunctionLibrary::SetRecentlyReceivedDamageTag(AActor* TargetActor, const FGameplayTag DamageType)
{
	if (AFQFCharacterBase* TargetCharacter = Cast<AFQFCharacterBase>(TargetActor))
	{
		TargetCharacter->RecentlyReceivedDamageType = DamageType;
	}
}

FGameplayEffectContextHandle UFQFBlueprintFunctionLibrary::ApplyDamageEffect(
	const FDamageEffectParams& DamageEffectParams)
{
	if (DamageEffectParams.TargetAbilitySystemComponent == nullptr || DamageEffectParams.SourceAbilitySystemComponent == nullptr) return FGameplayEffectContextHandle();

	AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	AActor* TargetAvatarActor = DamageEffectParams.TargetAbilitySystemComponent->GetAvatarActor();

	const FFQFGameplayTags GameplayTags = FFQFGameplayTags::Get();

	if(SourceAvatarActor == nullptr || TargetAvatarActor == nullptr) return FGameplayEffectContextHandle();

	//return empty if friendly fire and not poison damage type
	if (!IsNotFriend(SourceAvatarActor,TargetAvatarActor) && DamageEffectParams.DamageType != GameplayTags.DamageType_Poison) return FGameplayEffectContextHandle();
	if (IsTargetImmuneToDamageType(TargetAvatarActor, DamageEffectParams.DamageType)) return FGameplayEffectContextHandle();
	if (IsDead(TargetAvatarActor)) return FGameplayEffectContextHandle();

	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);
	SetKnockbackChance(EffectContextHandle, DamageEffectParams.KnockbackChance);
	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);


	if (DamageEffectParams.DamageType == GameplayTags.DamageType_Fluff)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.ExplosionChance, DamageEffectParams.ExplosionChance);
	}

	SetRecentlyReceivedDamageTag(TargetAvatarActor, DamageEffectParams.DamageType);

	const FActiveGameplayEffectHandle ActiveEffectHandle = DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

	//For infinite effects such as insect and poison damage, need to be added to an array on the Source and, for poison, Target Characters so they can be removed later on death or other event.
	if (SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite)
	{
		if (AFQFCharacterBase* SourceFQFCharacter = Cast<AFQFCharacterBase>(SourceAvatarActor))
		{
			SourceFQFCharacter->ActiveInfiniteEffectHandles.Add(ActiveEffectHandle, DamageEffectParams.TargetAbilitySystemComponent);
		}
		if (DamageEffectParams.DamageType == GameplayTags.DamageType_Poison)
		{
			if (AFQFCharacterBase* TargetFQFCharacter = Cast<AFQFCharacterBase>(TargetAvatarActor))
			{
				TargetFQFCharacter->ActiveInfiniteEffectHandles.Add(ActiveEffectHandle, DamageEffectParams.TargetAbilitySystemComponent);
			}

		}
	}
	// if (SpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::HasDuration)
	// {
	// 	//For Poison Damage, need to add the active effect handle to the Target, so that it can be removed on death later.
	// 	if (AFQFCharacterBase* TargetCharacter = Cast<AFQFCharacterBase>(TargetAvatarActor))
	// 	{
	// 		TargetCharacter->ActiveDurationEffectHandles.Add(ActiveEffectHandle, DamageEffectParams.TargetAbilitySystemComponent);
	// 	}
	// }


	return EffectContextHandle;
	
	
}

UNiagaraComponent* UFQFBlueprintFunctionLibrary::SpawnNiagaraAtPoint(const UObject* WorldContextObject, FVector InLocation, AActor* Actor, ESpawnPoint SpawnPoint,
	FVector Scale, UNiagaraSystem* NiagaraSystem)
{
		
	FVector Location = InLocation;

	if (Actor != nullptr)
	{
		
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Actor))
		{
			Location = Actor->GetActorLocation();
			if (SpawnPoint == ESpawnPoint::Ground)
			{
				Location = CombatInterface->Execute_GetActorGroundPoint(Actor);
			}
			else if (SpawnPoint == ESpawnPoint::Halo)
			{
				Location = CombatInterface->Execute_GetActorHaloPoint(Actor);
			}
		}
	}
	return UNiagaraFunctionLibrary::SpawnSystemAtLocation(WorldContextObject, NiagaraSystem,Location,FRotator::ZeroRotator,Scale);

}

UAbilityInfo* UFQFBlueprintFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AFQFGameModeBase* AuraGameMode = Cast<AFQFGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr ) return nullptr;

	return AuraGameMode->AbilityInfo;
}

AFQFGameModeBase* UFQFBlueprintFunctionLibrary::GetFQFGameMode(const UObject* WorldContextObject)
{
	return Cast<AFQFGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
}

UFQFGameInstance* UFQFBlueprintFunctionLibrary::GetGameInstance(const UObject* WorldContextObject)
{
	UFQFGameInstance* GameInstance = Cast<UFQFGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));

	return GameInstance;
}

FVector UFQFBlueprintFunctionLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UFQFBlueprintFunctionLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}
float UFQFBlueprintFunctionLibrary::GetKnockbackChance(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->GetKnockbackChance();
	}
	return 0.f;
}
bool UFQFBlueprintFunctionLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->IsRadialDamage();
	}
	return false;
}

float UFQFBlueprintFunctionLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UFQFBlueprintFunctionLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UFQFBlueprintFunctionLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FFQFGameplayEffectContext* FQFEffectContext = static_cast<const FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return FQFEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}


void UFQFBlueprintFunctionLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulse)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UFQFBlueprintFunctionLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InForce)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetKnockbackForce(InForce);
	}
}

void UFQFBlueprintFunctionLibrary::SetKnockbackChance(FGameplayEffectContextHandle& EffectContextHandle,
	const float InChance)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetKnockbackChance(InChance);
	}
}
void UFQFBlueprintFunctionLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UFQFBlueprintFunctionLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InInnerRadius)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UFQFBlueprintFunctionLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InOuterRadius)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UFQFBlueprintFunctionLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InOrigin)
{
	if (FFQFGameplayEffectContext* FQFEffectContext = static_cast<FFQFGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		FQFEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}


