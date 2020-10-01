#include "TODGameMode.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"
#include "TODPlayerState.h"
#include "TODUserWidget.h"

ATODGameMode::ATODGameMode()
{
	DefaultPawnClass = ATODCharacter::StaticClass();
	PlayerControllerClass = ATODPlayerController::StaticClass();
	PlayerStateClass = ATODPlayerState::StaticClass();

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

// 플레이어 컨트롤러의 구성을 완료하는 시점
void ATODGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	auto TODPlayerState = Cast<ATODPlayerState>(NewPlayer->PlayerState);
	if (TODPlayerState != nullptr)
		TODPlayerState->InitPlayerData();
}