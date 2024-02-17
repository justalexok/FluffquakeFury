// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PippaSpawnPoint.generated.h"

class UArrowComponent;

UCLASS()
class FLUFFQUAKEFURY_API APippaSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	APippaSpawnPoint();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* Arrow; 

};
