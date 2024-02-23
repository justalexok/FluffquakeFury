// Copyright Alex Goulder


#include "Character/FQFCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
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

	if (GetCharacterMovement() != nullptr)
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
void AFQFCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	HandleDeath();
}

void AFQFCharacterBase::SetMaxWalkSpeed_Implementation(bool bShouldImmobiliseCharacter)
{
	GetCharacterMovement()->MaxWalkSpeed = bShouldImmobiliseCharacter ? 0.f : BaseWalkSpeed;
}

FOnDeathSignature& AFQFCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

void AFQFCharacterBase::ResetRecentlyReceivedDamageType_Implementation()
{
	RecentlyReceivedDamageType = FGameplayTag();
}

UAnimInstance* AFQFCharacterBase::GetAnimInstance_Implementation()
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

void AFQFCharacterBase::KnockbackCharacter_Implementation(float Magnitude, float Pitch, FVector Direction)
{
	// const FVector OppositeDirection = GetActorForwardVector() * -1.0f;
	FRotator Rotation = Direction.Rotation();
	Rotation.Pitch = Pitch;
	const FVector ToTarget = Rotation.Vector();
	const FVector KnockbackForce = ToTarget * Magnitude;
	LaunchCharacter(KnockbackForce, true, true);
}

FOnDamageSignature& AFQFCharacterBase::GetOnDamageSignature()
{
	return OnDamageDelegate;
}



// float AFQFCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
//                                     AActor* DamageCauser)
// {
// 	const float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
// 	OnDamageDelegate.Broadcast(DamageTaken);
// 	return DamageTaken;
// }

FVector AFQFCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& SpecificAbilityTag)
{

	// Look up GameplayAbility in AbilityInfo and Retrieve The SocketLocation. 
	if (UAbilityInfo* AbilityInfo = UFQFBlueprintFunctionLibrary::GetAbilityInfo(this))
	{
		for (FFQFAbilityInfo Info : AbilityInfo->AbilityInformation)
		{
			if (Info.AbilityTag.MatchesTagExact(SpecificAbilityTag))
			{
				if (Info.bUsesWeapon && IsValid(Weapon))
				{
					return Weapon->GetSocketLocation(Info.SocketTagName);
					// DrawDebugSphere(GetWorld(),SL,12,64,FColor::Blue,false,20);
				}
				//Does not use weapon
				return GetMesh()->GetSocketLocation(Info.SocketTagName);
			}
		}
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





