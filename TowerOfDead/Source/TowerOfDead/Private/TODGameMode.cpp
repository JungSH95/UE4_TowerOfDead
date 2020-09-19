#include "TODGameMode.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"

ATODGameMode::ATODGameMode()
{
	DefaultPawnClass = ATODCharacter::StaticClass();
	PlayerControllerClass = ATODPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> BP_PAWN_C(TEXT("/Game/BluePrint/BP_Player.BP_Player_C"));
	if (BP_PAWN_C.Succeeded())
		DefaultPawnClass = BP_PAWN_C.Class;
}