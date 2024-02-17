// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "PippaPlayerStart.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLUFFQUAKEFURY_API UPippaPlayerStart : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPippaPlayerStart();

protected:
	virtual void BeginPlay() override;

	
};
