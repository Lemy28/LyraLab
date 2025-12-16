// CopyRight Lemy. All Rights Reserved.


#include "Input/LabInputComponent.h"
// #include "Input/LLInputConfig.h"


// Sets default values for this component's properties
ULabInputComponent::ULabInputComponent()
{

}

// void ULLInputComponent::AddInputMappings(const ULLInputConfig* InputConfig,
// 	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
// {
// 	check(InputConfig);
// 	check(InputSubsystem);
// }
//
// void ULLInputComponent::RemoveInputMappings(const ULLInputConfig* InputConfig,
// 	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
// {
// 	check(InputConfig);
// 	check(InputSubsystem);
//
// 	// Here you can handle any custom logic to remove input mappings that you may have added above
// }

void ULabInputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}

