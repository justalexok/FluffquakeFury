// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "Character/FQFCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyBase.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AEnemyBase : public AFQFCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

	AEnemyBase();
protected:
	virtual void BeginPlay() override;

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
};
