// Copyright Alex Goulder


#include "AbilitySystem/FQFBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/FQFPlayerState.h"
#include "UI/FQFWidgetController.h"
#include "UI/HUD/FQFHUD.h"


UOverlayWidgetController* UFQFBlueprintFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AFQFHUD* FQFHUD = Cast<AFQFHUD>(PC->GetHUD()))
		{
			AFQFPlayerState* PS = PC->GetPlayerState<AFQFPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			
			return FQFHUD->GetOverlayWidgetController(WidgetControllerParams);
			
		}
	}
	return nullptr;
}
