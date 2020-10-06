#include "TODGameInstance.h"

UTODGameInstance::UTODGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/CharacterData.CharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTER(*CharacterDataPath);
	if (DT_CHARACTER.Succeeded())
		TODCharacterTable = DT_CHARACTER.Object;
}

void UTODGameInstance::Init()
{
	Super::Init();
	
	TODLOG(Warning, TEXT("DropExp of Level 10 : %d"), GetTODCharacterData(10)->NextSoul);
}

FTODCharacterData* UTODGameInstance::GetTODCharacterData(int32 Level)
{
	return TODCharacterTable->FindRow<FTODCharacterData>(*FString::FromInt(Level), TEXT(""));
}