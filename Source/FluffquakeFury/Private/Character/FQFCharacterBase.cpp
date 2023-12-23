// Copyright Alex Goulder


#include "Character/FQFCharacterBase.h"

#include "AbilitySystem/FQFAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AFQFCharacterBase::AFQFCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AFQFCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;

	InitAbilityActorInfo();
	
}

void AFQFCharacterBase::InitAbilityActorInfo()
{
}

