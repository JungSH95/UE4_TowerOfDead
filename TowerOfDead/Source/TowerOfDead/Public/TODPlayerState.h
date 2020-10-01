#pragma once

#include "TowerOfDead.h"
#include "GameFramework/PlayerState.h"
#include "TODPlayerState.generated.h"



UCLASS()
class TOWEROFDEAD_API ATODPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATODPlayerState();

	void InitPlayerData();

	int32 GetTotalLevel() const { return TotalLevel; }
	int32 GetHPLevel() const { return HPLevel; }
	int32 GetATKLevel() const { return ATKLevel; }
	int32 GetDEFLevel() const { return DEFLevel; }
	int32 GetSoul() const { return CurrentSoul; }

	FString SaveSlotName;

protected:
	UPROPERTY(Transient)
	int32 TotalLevel;

	UPROPERTY(Transient)
	int32 HPLevel;

	UPROPERTY(Transient)
	int32 ATKLevel;

	UPROPERTY(Transient)
	int32 DEFLevel;

	UPROPERTY(Transient)
	int32 CurrentSoul;
};
