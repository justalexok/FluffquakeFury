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

public:
	//EnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

};
