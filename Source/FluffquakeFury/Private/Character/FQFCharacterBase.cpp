// Copyright Alex Goulder


#include "Character/FQFCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/FQFAbilitySystemComponent.h"
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
}

void AFQFCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
}

FVector AFQFCharacterBase::GetCombatSocketLocation_Implementation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

bool AFQFCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AFQFCharacterBase::GetAvatar_Implementation()
{
	return this;
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

void AFQFCharacterBase::AddCharacterAbilities()
{
	UFQFAbilitySystemComponent* FQFASC = CastChecked<UFQFAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	FQFASC->AddCharacterAbilities(StartupAbilities);
	
}

void AFQFCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultVitalAttributes, 1);
}

