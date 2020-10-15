#pragma once

#include "TowerOfDead.h"
#include "GameFramework/PlayerState.h"
#include "TODPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

UCLASS()
class TOWEROFDEAD_API ATODPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ATODPlayerState();

	void InitPlayerData();
	void SavePlayerData();

	int32 GetTotalLevel() const { return TotalLevel; }
	int32 GetHPLevel() const { return HPLevel; }
	int32 GetATKLevel() const { return ATKLevel; }
	int32 GetDEFLevel() const { return DEFLevel; }
	int32 GetCurrentSoul() const { return CurrentSoul; }

	void AddSoul(int32 Soul);

	FString SaveSlotName;

	FOnPlayerStateChangedDelegate OnPlayerStateChange;

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 TotalLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 HPLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 ATKLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 DEFLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = State, Meta = (AllowPrivateAccess = true))
	int32 CurrentSoul;
};
