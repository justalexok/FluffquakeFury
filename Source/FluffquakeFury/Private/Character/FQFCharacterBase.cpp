// Copyright Alex Goulder


#include "Character/FQFCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "FQFGameplayTags.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFQFCharacterBase::AFQFCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("CharacterAudioComponent"));
	AudioComponent->bAutoActivate = false;
}


void AFQFCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetCharacterMovement() == nullptr)
	{
		
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	}
	
}

void AFQFCharacterBase::HandleDeath()
{

	AudioComponent->Deactivate();
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();

	bDead = true;

	OnDeathDelegate.Broadcast(this); //Currently nobody listening

}
void AFQFCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	HandleDeath();
}

void AFQFCharacterBase::SetMaxWalkSpeed_Implementation(bool bShouldImmobiliseCharacter)
{
	GetCharacterMovement()->MaxWalkSpeed = bShouldImmobiliseCharacter ? 0.f : BaseWalkSpeed;
}

TArray<FTaggedMontage> AFQFCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

FTaggedMontage AFQFCharacterBase::GetAttackMontageWithTag_Implementation(FGameplayTag MontageTag)
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return  TaggedMontage;
		}
	}
	return FTaggedMontage();
}

FOnDeathSignature& AFQFCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

void AFQFCharacterBase::ResetRecentlyReceivedDamageType_Implementation()
{
	RecentlyReceivedDamageType = FGameplayTag();
}

UAnimInstance* AFQFCharacterBase::GetAnimInstance_Implementation(AActor* Actor)
{
	return GetMesh()->GetAnimInstance();
}

FVector AFQFCharacterBase::GetActorGroundPoint_Implementation()
{
	FVector GroundLocation = GetActorLocation();
	GroundLocation.Z = GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return GroundLocation;
	
}

FVector AFQFCharacterBase::GetActorHaloPoint_Implementation()
{
	FVector GroundLocation = GetActorLocation();
	GroundLocation.Z = GetActorLocation().Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	return GroundLocation;
}

void AFQFCharacterBase::SetWeaponVisibility_Implementation(bool bVisible)
{	
	Weapon->SetVisibility(bVisible);
}

FVector AFQFCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FFQFGameplayTags& GameplayTags = FFQFGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Jump))
	{
		return GetMesh()->GetSocketLocation(CenterChesterSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_PillowWhack) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName); 
	}
	return FVector();
}

bool AFQFCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AFQFCharacterBase::GetAvatar_Implementation()
{
	return this;
}

UAnimMontage* AFQFCharacterBase::GetHitReactMontage_Implementation()
{
	if (DamageTypesToHitReactMontages.FindRef(RecentlyReceivedDamageType))
	{
		UE_LOG(LogTemp, Display, TEXT("Recently Received Damage Type: %s"), *RecentlyReceivedDamageType.ToString());
		return DamageTypesToHitReactMontages[RecentlyReceivedDamageType];
	}	
	return nullptr;
	
}


void AFQFCharacterBase::InitAbilityActorInfo()
{
}

void AFQFCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AFQFCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultVitalAttributes, 1);
}

void AFQFCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		//Loop over each Material in the SKM and swap for DynamicMatInst
		for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); ++i)
		{
			GetMesh()->SetMaterial(i, DynamicMatInst);
		}	
		StartDissolveTimeline(DynamicMatInst);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		for (int32 i = 0; i < GetMesh()->GetMaterials().Num(); ++i)
		{
			Weapon->SetMaterial(i, DynamicMatInst);
		}
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}





