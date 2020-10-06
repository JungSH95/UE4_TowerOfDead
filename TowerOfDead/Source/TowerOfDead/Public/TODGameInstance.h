#pragma once

#include "TowerOfDead.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "TODGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FTODCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTODCharacterData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextSoul;
};

USTRUCT(BlueprintType)
struct FTODEnemyData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FTODEnemyData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropSoul;
};

UCLASS()
class TOWEROFDEAD_API UTODGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTODGameInstance();

	virtual void Init() override;
	FTODCharacterData* GetTODCharacterData(int32 Level);
	FTODEnemyData* GetTODEnemyData(int32 Level);

private:
	class UDataTable* TODCharacterTable;
	class UDataTable* TODEnemyTable;
};
