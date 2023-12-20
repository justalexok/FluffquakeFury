// Copyright Alex Goulder


#include "Character/Enemy/EnemyBase.h"
#include "FluffquakeFury/FluffquakeFury.h"

AEnemyBase::AEnemyBase()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyBase::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AEnemyBase::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}
