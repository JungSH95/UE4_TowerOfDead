#include "TODEnemyWidget.h"
#include "TODEnemyStatComponent.h"
#include "Components/ProgressBar.h"

void UTODEnemyWidget::BindEnemyStat(class UTODEnemyStatComponent* NewEnemyStat)
{
	if (NewEnemyStat == nullptr)
		return;

	CurrentEnemyStat = NewEnemyStat;
	CurrentEnemyStat->OnHPChanged.AddUObject(this, &UTODEnemyWidget::UpdateHPWidget);

	TODLOG_S(Warning);
}

void UTODEnemyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	UpdateHPWidget();
}

void UTODEnemyWidget::UpdateHPWidget()
{
	if (CurrentEnemyStat == nullptr)
		return;
	
	TODLOG_S(Warning);

	if (HPProgressBar != nullptr)
		HPProgressBar->SetPercent(CurrentEnemyStat->GetHPRatio());
}