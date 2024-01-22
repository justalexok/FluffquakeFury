// Copyright Alex Goulder


#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "GameFramework/Character.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Character/FQFCharacterBase.h"
#include "Net/UnrealNetwork.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PippaPlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

UFQFAttributeSet::UFQFAttributeSet()
{

}

void UFQFAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, Fluff, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, MaxFluff, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, LoadedFluff, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFQFAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);


}

void UFQFAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
		UE_LOG(LogTemp,Warning,TEXT("Changed Health on %s, Health %f"), *Props.TargetAvatarActor->GetName(), GetHealth());
	}
	if (Data.EvaluatedData.Attribute == GetFluffAttribute())
	{		
		SetFluff(FMath::Clamp(GetFluff(), 0.f, GetMaxFluff()));			
	}
	if (Data.EvaluatedData.Attribute == GetLoadedFluffAttribute())
	{		
		SetLoadedFluff(FMath::Clamp(GetLoadedFluff(), 0.f, 100));			
	}
	
	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{

		const bool bPippaPillowAttack = Props.AttackTag == FFQFGameplayTags::Get().Montage_Attack_PillowWhack;
		
		const float LocalIncomingDamage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		if (LocalIncomingDamage > 0.f)
		{
			const float NewHealth = GetHealth() - LocalIncomingDamage;
			SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));
			UE_LOG(LogTemp,Warning,TEXT("Incoming Damage on %s, Damage: %f, New Health %f"), *Props.TargetAvatarActor->GetName(),LocalIncomingDamage, GetHealth());

			const bool bFatal = GetHealth() <= 0.f;

			if (bFatal)
			{
				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
				{
					CombatInterface->Die();
				}
			}
			else
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FFQFGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}
			if (bPippaPillowAttack)
			{
				SetFluffLost(Props, LocalIncomingDamage);			
				SpawnNiagara(Props.SourceCharacter, false, LocalIncomingDamage);
			}
			const bool bBlocked = UFQFBlueprintFunctionLibrary::IsBlockedHit(Props.EffectContextHandle);	
			ShowFloatingText(Props, LocalIncomingDamage, bBlocked, false);
			

			
		}
		if (bPippaPillowAttack && UFQFBlueprintFunctionLibrary::HasPillowExploded(Props.EffectContextHandle))
		{
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FFQFGameplayTags::Get().Effects_HitReact);
			Props.SourceASC->TryActivateAbilitiesByTag(TagContainer);
			
			HandleExplosion(Props,LocalIncomingDamage);
		}
		
	}
}


void UFQFAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}		
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}

	//TODO Get Attack type from Data?
	if (Props.SourceCharacter->ActorHasTag("Player"))
	{
		Props.AttackTag = FFQFGameplayTags::Get().Montage_Attack_PillowWhack;
	}
}

void UFQFAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit, bool bPillowExploded)
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		//Pippa causing damage
		if(APippaPlayerController* PC = Cast<APippaPlayerController>(UGameplayStatics::GetPlayerController(Props.SourceCharacter, 0)))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bPillowExploded);
			return;
		}
		//Enemy causing damage
		if(APippaPlayerController* PC = Cast<APippaPlayerController>(UGameplayStatics::GetPlayerController(Props.TargetCharacter, 0)))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bPillowExploded);
		}
	
	}
	else
	{
		//Source and Target the same - must be Effect actor causing Damage to Pippa? 
		if(APippaPlayerController* PC = Cast<APippaPlayerController>(UGameplayStatics::GetPlayerController(Props.SourceCharacter, 0)))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bPillowExploded);
			
		}
	}
}

void UFQFAttributeSet::SpawnNiagara(ACharacter* SourceCharacter, bool bPillowExploded, float Damage) const
{
	
	if (AFQFCharacterBase* FQFCharacterBase = Cast<AFQFCharacterBase>(SourceCharacter))
	{
		const FVector SpawnLocation = FQFCharacterBase->GetCombatSocketLocation_Implementation(FFQFGameplayTags::Get().Montage_Attack_PillowWhack);

		TObjectPtr<UNiagaraSystem> Effect;
		if (bPillowExploded) Effect = FQFCharacterBase->Pillow->ExplosionEffect;
		else Effect = FQFCharacterBase->Pillow->DynamicImpactEffect;

		const float ScaledSpawnRate = FQFCharacterBase->Pillow->SpawnRate.GetValueAtLevel(Damage);
		const FLinearColor FluffColor = FQFCharacterBase->Pillow->FluffColor;
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Effect, SpawnLocation);
		NiagaraComponent->SetNiagaraVariableFloat(FString("CustomSpawnRate"),ScaledSpawnRate);
		NiagaraComponent->SetNiagaraVariableLinearColor(FString("CustomColor"),FluffColor);

	}
	
}

void UFQFAttributeSet::HandleExplosion(const FEffectProperties& Props, float LocalIncomingDamage) const
{
	SpawnNiagara(Props.SourceCharacter, true, 100);

	if (UFQFAttributeSet* SourceAS = UFQFBlueprintFunctionLibrary::GetAttributeSet(Props.SourceAvatarActor))
	{
		//Reduce fluff by LoadedFluff amount. Set Loaded Fluff to zero. 
		SourceAS->SetFluff(SourceAS->GetFluff() - SourceAS->GetLoadedFluff());
		SourceAS->SetLoadedFluff(0);		
				
	}
	ShowFloatingText(Props, LocalIncomingDamage, false, true);
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.SourceCharacter))
	{
		CombatInterface->KnockbackCharacter(25,1100);
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(FFQFGameplayTags::Get().Effects_HitReact);
		Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
	}
}

void UFQFAttributeSet::SetFluffLost(const FEffectProperties& Props, float LocalIncomingDamage) const
{
	if (UFQFAttributeSet* SourceAS = UFQFBlueprintFunctionLibrary::GetAttributeSet(Props.SourceAvatarActor))
	{
		float DamageToFluffLostCoeff = 0.1;
		const float FluffLost = LocalIncomingDamage * DamageToFluffLostCoeff;
		SourceAS->SetFluff(SourceAS->GetFluff() - FluffLost);
		SourceAS->SetLoadedFluff(SourceAS->GetLoadedFluff() - FluffLost);
	}
}

void UFQFAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, Health, OldHealth);
}

void UFQFAttributeSet::OnRep_Fluff(const FGameplayAttributeData& OldFluff) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, Fluff, OldFluff);

}

void UFQFAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, Fluff, OldMaxHealth);

}

void UFQFAttributeSet::OnRep_MaxFluff(const FGameplayAttributeData& OldMaxFluff) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, Fluff, OldMaxFluff);

}

void UFQFAttributeSet::OnRep_LoadedFluff(const FGameplayAttributeData& OldLoadedFluff) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, LoadedFluff, OldLoadedFluff);

}

void UFQFAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, Strength, OldStrength);
}
void UFQFAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, Resilience, OldResilience);
}

void UFQFAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFQFAttributeSet, BlockChance, OldBlockChance);
}

