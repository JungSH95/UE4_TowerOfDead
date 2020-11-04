#pragma once

#include "TowerOfDead.h"
#include "Blueprint/UserWidget.h"
#include "TODLevelUpWidget.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODLevelUpWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void BindPlayerStateClass(class ATODPlayerState* playerState);
	void BindCharacterStatClass(class UTODCharacterStatComponent* characterStat);

	void UpdateNowPlayerState();
	void UpdateNextPlayerState();

	UFUNCTION()
	void OnHPDownClicked();
	UFUNCTION()
	void OnHPUpClicked();

	UFUNCTION()
	void OnATKDownClicked();
	UFUNCTION()
	void OnATKUpClicked();

	UFUNCTION()
	void OnDEFDownClicked();
	UFUNCTION()
	void OnDEFUpClicked();

	UFUNCTION()
	void OnActionClicked();

private:
	TWeakObjectPtr<class UTODCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ATODPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UTextBlock* NowTotalLevel;
	UPROPERTY()
	class UTextBlock* NextTotalLevel;

	UPROPERTY()
	class UTextBlock* NowSoul;
	UPROPERTY()
	class UTextBlock* NextSoul;
	UPROPERTY()
	class UTextBlock* NeedSoul;

	UPROPERTY()
	class UTextBlock* NowHPLevel;
	UPROPERTY()
	class UTextBlock* NextHPLevel;

	UPROPERTY()
	class UTextBlock* NowATKLevel;
	UPROPERTY()
	class UTextBlock* NextATKLevel;

	UPROPERTY()
	class UTextBlock* NowDEFLevel;
	UPROPERTY()
	class UTextBlock* NextDEFLevel;

	UPROPERTY()
	class UTextBlock* NowHP;
	UPROPERTY()
	class UTextBlock* NextHP;

	UPROPERTY()
	class UTextBlock* NowATK;
	UPROPERTY()
	class UTextBlock* NextATK;

	UPROPERTY()
	class UTextBlock* NowDEF;
	UPROPERTY()
	class UTextBlock* NextDEF;

	UPROPERTY()
	class UButton* HP_DownButton;

	UPROPERTY()
	class UButton* HP_UpButton;

	UPROPERTY()
	class UButton* ATK_DownButton;

	UPROPERTY()
	class UButton* ATK_UpButton;

	UPROPERTY()
	class UButton* DEF_DownButton;

	UPROPERTY()
	class UButton* DEF_UpButton;

	UPROPERTY()
	class UButton* Action_Button;

	UPROPERTY()
	int32 HPCount;

	UPROPERTY()
	int32 ATKCount;

	UPROPERTY()
	int32 DEFCount;
};
