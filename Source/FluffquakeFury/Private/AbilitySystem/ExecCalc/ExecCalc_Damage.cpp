// Copyright Alex Goulder


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAttributeSet.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"


struct FQFDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Resilience);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LoadedFluff);


	FQFDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, Resilience, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UFQFAttributeSet, LoadedFluff, Source, false);

	}
};

static const FQFDamageStatics& DamageStatics()
{
	static FQFDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ResilienceDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LoadedFluffDef);

}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FFQFGameplayTags GameplayTags =  FFQFGameplayTags::Get();

	//Get Damage Set By Caller Magnitude
	float Damage = 0.f;
	TArray<FGameplayTag> DamageTypes =GameplayTags.DamageTypes;
	FGameplayTag LocalDamageType; 
	for (const FGameplayTag DamageTypeTag : DamageTypes)
	{
		//Will find zero for all damage types other than the type that this damage is
		//i.e. Only one type tag will have its magnitude set.
		float DamageTypeAmount= Spec.GetSetByCallerMagnitude(DamageTypeTag, false);

		if (DamageTypeAmount != 0)
		{
			Damage += DamageTypeAmount;
			LocalDamageType = DamageTypeTag; //Set Damage Type
		}
		
	}
	UE_LOG(LogTemp,Display,TEXT("Damage Type: %s"),*LocalDamageType.ToString());	
	
	//Capture BlockChance on Target and determine if successful block
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);	
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UFQFBlueprintFunctionLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);
	UFQFBlueprintFunctionLibrary::SetDamageType(EffectContextHandle, LocalDamageType);

	if (LocalDamageType == GameplayTags.DamageType_Fluff)
	{
		//Capture LoadedFluff on Source 
		float SourceLoadedFluff = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().LoadedFluffDef, EvaluationParameters, SourceLoadedFluff);

		//TODO Capture Strength 

		//Get ExplosionChance Set By Caller Magnitude and determine if explosion
		const float ExplosionChance = Spec.GetSetByCallerMagnitude(FFQFGameplayTags::Get().ExplosionChance, false);
		UE_LOG(LogTemp, Display, TEXT("Explosion Chance : %f"), ExplosionChance);
		const bool bExploded = FMath::RandRange(1, 100) < ExplosionChance;

		UFQFBlueprintFunctionLibrary::SetHasPillowExploded(EffectContextHandle, bExploded);

		//Alter Damage
		Damage *= SourceLoadedFluff / 100;
		//TODO Alter based on strength
		Damage = bExploded ? 0 : Damage;
		if (bExploded) UE_LOG(LogTemp, Error, TEXT("PILLOW EXPLODED!"));

	}
	UE_LOG(LogTemp,Display, TEXT("Damage before bBlocked: %f"),Damage);

	
	Damage = bBlocked ? Damage / 2.f : Damage;

	
	if (UFQFBlueprintFunctionLibrary::IsRadialDamage(EffectContextHandle))
	{
		//Get distance from Source to Target. Multiply by Base Damage 

		FVector TargetLocation = TargetAvatar->GetActorLocation();		
		FVector Origin = UFQFBlueprintFunctionLibrary::GetRadialDamageOrigin(EffectContextHandle);
		float OriginToTargetDistance = FVector::Distance(Origin,TargetLocation);
		//Closer to Target, more damage done.
		Damage = Damage * 5 / OriginToTargetDistance;

	}
	
	Damage = round(Damage);

	

	UE_LOG(LogTemp,Display, TEXT("Final Damage in ExecCalc: %f"),Damage);

	
	const FGameplayModifierEvaluatedData IncomingDamageEvaluatedData(UFQFAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(IncomingDamageEvaluatedData);


}
