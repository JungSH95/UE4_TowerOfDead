#include "TODPlayerState.h"
#include "TODSaveGame.h"

ATODPlayerState::ATODPlayerState()
{
	TotalLevel = 1;
	HPLevel = 1;
	ATKLevel = 1;
	DEFLevel = 1;
	CurrentSoul = 0;

	SaveSlotName = TEXT("Player1");
}

void ATODPlayerState::InitPlayerData()
{
	/*
	auto LoadData = Cast<UTODSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (LoadData == nullptr)
	{

	}
	*/
}