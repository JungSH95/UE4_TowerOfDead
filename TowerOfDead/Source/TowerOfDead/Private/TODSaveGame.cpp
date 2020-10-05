#include "TODSaveGame.h"

UTODSaveGame::UTODSaveGame()
{
	PlayerState.TotalLevel = 1;
	PlayerState.HPLevel = 1;
	PlayerState.ATKLevel = 1;
	PlayerState.DEFLevel = 1;
	PlayerState.CurrentSoul = 0;

	PlayerName = TEXT("Guest");
}

void UTODSaveGame::SetPlayerState(int32 totallevel, int32 hplevel, int32 atklevel, int32 deflevel, int32 soul)
{
	PlayerState.TotalLevel = totallevel;
	PlayerState.HPLevel = hplevel;
	PlayerState.ATKLevel = atklevel;
	PlayerState.DEFLevel = deflevel;
	PlayerState.CurrentSoul = soul;
}