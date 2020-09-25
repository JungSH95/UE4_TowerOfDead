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
	void BindPlayerClass(class ATODCharacter* character);
	void UpdateHardAttackCast();

	void SetVisibleCast(bool isVisible);

private:
	TWeakObjectPtr<class ATODCharacter> Player;

	UPROPERTY()
	class UProgressBar* HardAttackCastBar;

	UPROPERTY()
	class UUserWidget* CastWidget;
};
