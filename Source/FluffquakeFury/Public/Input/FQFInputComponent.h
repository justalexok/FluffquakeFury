// Copyright Alex Goulder

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "FQFInputConfig.h"
#include "FQFInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class FLUFFQUAKEFURY_API UFQFInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UFQFInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
	
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UFQFInputComponent::BindAbilityActions(const UFQFInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FFQFInputAction& Action: InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
