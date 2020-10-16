#pragma once

#include "TowerOfDead.h"
#include "Blueprint/UserWidget.h"
#include "TODEnemyWidget.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindEnemyStat(class UTODEnemyStatComponent* NewEnemyStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UTODEnemyStatComponent> CurrentEnemyStat;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
