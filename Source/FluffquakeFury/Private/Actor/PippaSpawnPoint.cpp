// Copyright Alex Goulder


#include "Actor/PippaSpawnPoint.h"

#include "Components/ArrowComponent.h"

APippaSpawnPoint::APippaSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	Arrow->SetupAttachment(GetRootComponent());

}



