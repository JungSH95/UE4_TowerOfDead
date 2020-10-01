#pragma once

#include "TowerOfDead.h"
#include "GameFramework/SaveGame.h"
#include "TODSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FPlayerStatStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stat)
	int32 TotalLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stat)
	int32 HPLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stat)
	int32 ATKLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stat)
	int32 DEFLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Stat)
	int32 CurrentSoul;
};

UCLASS()
class TOWEROFDEAD_API UTODSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	FPlayerStatStruct PlayerState;
};
