#include "TODGameInstance.h"

UTODGameInstance::UTODGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/CharacterData.CharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTER(*CharacterDataPath);
	if (DT_CHARACTER.Succeeded())
		TODCharacterTable = DT_CHARACTER.Object;

	FString EnemyDataPath = TEXT("/Game/GameData/EnemyData.EnemyData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ENEMY(*EnemyDataPath);
	if (DT_ENEMY.Succeeded())
		TODEnemyTable = DT_ENEMY.Object;
}

void UTODGameInstance::Init()
{
	Super::Init();
	
	//TODLOG(Warning, TEXT("NextExp of Level 10 : %d"), GetTODCharacterData(10)->NextSoul);
	//TODLOG(Warning, TEXT("DropExp of Level 10 : %d"), GetTODEnemyData(10)->DropSoul);
}

FTODCharacterData* UTODGameInstance::GetTODCharacterData(int32 Level)
{
	return TODCharacterTable->FindRow<FTODCharacterData>(*FString::FromInt(Level), TEXT(""));
}

FTODEnemyData* UTODGameInstance::GetTODEnemyData(int32 Level)
{
	return TODEnemyTable->FindRow<FTODEnemyData>(*FString::FromInt(Level), TEXT(""));
}