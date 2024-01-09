// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FQFGameModeBase.generated.h"
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API AFQFGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
};
