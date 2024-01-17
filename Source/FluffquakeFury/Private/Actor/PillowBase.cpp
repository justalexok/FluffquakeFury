// Copyright Alex Goulder


#include "Actor/PillowBase.h"


APillowBase::APillowBase()
{
	// TestPillowMesh = CreateDefaultSubobject<USkeletalMeshComponent>("TestPillow");
	// TestPillowMesh->SetupAttachment(GetRootComponent(), FName("WeaponHandSocket"));
	// TestPillowMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APillowBase::BeginPlay()
{
	Super::BeginPlay();	
}


