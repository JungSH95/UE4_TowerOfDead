#pragma once

#include "TowerOfDead.h"
#include "Blueprint/UserWidget.h"
#include "TODUserWidget.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	void BindGameModeClass(class ATODGameMode* gamemode);
	void BindPlayerClass(class ATODCharacter* player);
	void BindCharacterStatClass(class UTODCharacterStatComponent* characterStat);
	void BindPlayerStateClass(class ATODPlayerState* playerState);
	void BindBossEnemyStateClass(class UTODEnemyStatComponent* enemyStat);

	void UpdateGameMode();
	void UpdateCharacterStat();
	void UpdatePlayerState();
	void UpdateSkillCast();
	void UpdateBossState();

	void SetVisibleLevelUp(bool isVisible);
	void SetVisibleCast(bool isVisible);
	void SetBossInfoVisible(bool isVisible);

private:
	TWeakObjectPtr<class ATODGameMode> CurrentGameMode;
	TWeakObjectPtr<class ATODCharacter> CurrentPlayer;
	TWeakObjectPtr<class UTODCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ATODPlayerState> CurrentPlayerState;
	TWeakObjectPtr<class UTODEnemyStatComponent> BossEnemyStat;

	UPROPERTY()
	class UTODLevelUpWidget* LevelUpWidget;

	UPROPERTY()
	class UTextBlock* CharacterName;

	UPROPERTY()
	class UTextBlock* CurrentStage;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UTextBlock* CurrentSoul;

	UPROPERTY()
	class UProgressBar* SkillCastBar;

	UPROPERTY()
	class UUserWidget* CastWidget;

	UPROPERTY()
	class UTextBlock* BossName;

	UPROPERTY()
	class UProgressBar* BossHPBar;
};
