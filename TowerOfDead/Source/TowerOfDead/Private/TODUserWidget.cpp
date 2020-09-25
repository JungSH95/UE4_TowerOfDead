#include "TODUserWidget.h"
#include "TODCharacter.h"
#include "Components/ProgressBar.h"

void UTODUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CastWidget = Cast<UUserWidget>(GetWidgetFromName(TEXT("CastBar_UI")));
	if (CastWidget != nullptr)
	{
		HardAttackCastBar = Cast<UProgressBar>(CastWidget->GetWidgetFromName(TEXT("CastBar")));
		CastWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UTODUserWidget::BindPlayerClass(class ATODCharacter* character)
{
	if (character != nullptr)
	{
		Player = character;
		Player->OnHardAttackCast.AddUObject(this, &UTODUserWidget::UpdateHardAttackCast);
		UpdateHardAttackCast();			
	}
}

void UTODUserWidget::UpdateHardAttackCast()
{
	if (HardAttackCastBar == nullptr)
		return;

	HardAttackCastBar->SetPercent(Player->GetHardAttackRatio());
}

void UTODUserWidget::SetVisibleCast(bool isVisible)
{
	if (CastWidget != nullptr)
	{
		if(isVisible)
			CastWidget->SetVisibility(ESlateVisibility::Visible);
		else
			CastWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}