#include "TODUIPlayerController.h"
#include "Blueprint/UserWidget.h"

void ATODUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UIWidgetClass == nullptr)
		return;

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	if (UIWidgetInstance == nullptr)
		return;

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}

