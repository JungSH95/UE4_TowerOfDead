#include "TODPlayerState.h"
#include "TODSaveGame.h"

ATODPlayerState::ATODPlayerState()
{
	TotalLevel = 3;
	HPLevel = 1;
	ATKLevel = 1;
	DEFLevel = 1;
	CurrentSoul = 0;

	SaveSlotName = TEXT("Player1");
}

void ATODPlayerState::InitPlayerData()
{
	auto LoadData = Cast<UTODSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (LoadData == nullptr)
		LoadData = GetMutableDefault<UTODSaveGame>();

	SetPlayerName(LoadData->PlayerName);
	TotalLevel = LoadData->PlayerState.TotalLevel;
	HPLevel = LoadData->PlayerState.HPLevel;
	ATKLevel = LoadData->PlayerState.ATKLevel;
	DEFLevel = LoadData->PlayerState.DEFLevel;
	CurrentSoul = LoadData->PlayerState.CurrentSoul;

	SavePlayerData();
}

void ATODPlayerState::SavePlayerData()
{
	UTODSaveGame* NewPlayerData = NewObject<UTODSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->SetPlayerState(
		TotalLevel,
		HPLevel,
		ATKLevel,
		DEFLevel,
		CurrentSoul
	);

	if (!UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
		TODLOG(Error, TEXT("Save Game Error!!!!!!!"));
}

void ATODPlayerState::LevelUp(int32 hp, int32 atk, int32 def)
{
	HPLevel += hp;
	ATKLevel += atk;
	DEFLevel += def;

	TotalLevel = HPLevel + ATKLevel + DEFLevel;
	SavePlayerData();

	print(FString::Printf(TEXT("Level Up OK")));
}

void ATODPlayerState::AddSoul(int32 Soul)
{
	CurrentSoul += Soul;

	if (CurrentSoul <= 0)
		CurrentSoul = 0;

	SavePlayerData();
}