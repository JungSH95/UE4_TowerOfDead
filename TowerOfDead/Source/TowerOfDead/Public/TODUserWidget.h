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
	void BindPlayerClass(class ATODCharacter* player);
	void BindCharacterStatClass(class UTODCharacterStatComponent* characterStat);
	void BindPlayerStateClass(class ATODPlayerState* playerState);

	void UpdateCharacterStat();
	void UpdatePlayerState();
	void UpdateHardAttackCast();

	void SetVisibleCast(bool isVisible);

private:
	TWeakObjectPtr<class ATODCharacter> CurrentPlayer;
	TWeakObjectPtr<class UTODCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ATODPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* HardAttackCastBar;

	UPROPERTY()
	class UUserWidget* CastWidget;
};
