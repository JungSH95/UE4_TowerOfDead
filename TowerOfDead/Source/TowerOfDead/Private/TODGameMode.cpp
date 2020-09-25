#include "TODGameMode.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"
#include "TODUserWidget.h"

ATODGameMode::ATODGameMode()
{
	DefaultPawnClass = ATODCharacter::StaticClass();
	PlayerControllerClass = ATODPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Game/BluePrint/BP_Player.BP_Player_C"));
	if (BP_PAWN_C.Succeeded())
		DefaultPawnClass = BP_PAWN_C.Class;

	static ConstructorHelpers::FClassFinder<UTODUserWidget> BP_UI(TEXT("/Game/BluePrint/InGameUI.InGameUI_C"));
	if (BP_UI.Succeeded())
		HUDWidgetClass = BP_UI.Class;

}

void ATODGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		HUDWidget = CreateWidget<UTODUserWidget>(GetWorld(), HUDWidgetClass);
		if (HUDWidget != nullptr)
			HUDWidget->AddToViewport();
	}
}