// Copyright Lemy. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LyraLabIndicatorManagerComponent.generated.h"


class UIndicatorDescriptor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LYRALAB_API ULyraLabIndicatorManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULyraLabIndicatorManagerComponent();

	static ULyraLabIndicatorManagerComponent* GetComponent(AController* Controller);
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void RemoveIndicator(UIndicatorDescriptor* Indicator);
	UFUNCTION(BlueprintCallable, Category = Indicator)
	void AddIndicator(UIndicatorDescriptor* Indicator);

	const TArray<UIndicatorDescriptor*>& GetIndicators() {return Indicators;};
	
	DECLARE_EVENT_OneParam(ULyraLabIndicatorManagerComponent, FIndicatorEvent, UIndicatorDescriptor* Indicator);

	FIndicatorEvent OnIndicatorAdded;
	FIndicatorEvent OnIndicatorRemoved;

private:
	TArray<TObjectPtr<UIndicatorDescriptor>> Indicators;
};
