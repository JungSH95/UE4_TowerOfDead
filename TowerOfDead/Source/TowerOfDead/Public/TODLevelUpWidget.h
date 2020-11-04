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

private:
	TWeakObjectPtr<class UTODCharacterStatComponent> CurrentCharacterStat;

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
};
