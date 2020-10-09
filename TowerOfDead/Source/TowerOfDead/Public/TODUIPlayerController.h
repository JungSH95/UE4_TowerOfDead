#pragma once

#include "TowerOfDead.h"
#include "GameFramework/PlayerController.h"
#include "TODUIPlayerController.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODUIPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
