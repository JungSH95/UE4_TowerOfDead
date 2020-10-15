#include "TODUserWidget.h"
#include "TODCharacter.h"
#include "TODCharacterStatComponent.h"
#include "TODPlayerState.h"
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

	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Player_HPBar")));
}

void UTODUserWidget::BindPlayerClass(class ATODCharacter* player)
{
	if (player != nullptr)
	{
		CurrentPlayer = player;
		CurrentPlayer->OnHardAttackCast.AddUObject(this, &UTODUserWidget::UpdateHardAttackCast);
		UpdateHardAttackCast();			
	}
}

void UTODUserWidget::BindCharacterStatClass(class UTODCharacterStatComponent* characterStat)
{
	if (characterStat != nullptr)
	{
		CurrentCharacterStat = characterStat;
	}
}

void UTODUserWidget::BindPlayerStateClass(class ATODPlayerState* playerState)
{
	if (playerState != nullptr)
	{
		CurrentPlayerState = playerState;
	}
}

void UTODUserWidget::UpdateCharacterStat()
{
	if (CurrentCharacterStat == nullptr)
		return;

	if (HPBar != nullptr)
		HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void UTODUserWidget::UpdatePlayerState()
{
	if (CurrentPlayerState == nullptr)
		return;

}

void UTODUserWidget::UpdateHardAttackCast()
{
	if (HardAttackCastBar == nullptr)
		return;

	HardAttackCastBar->SetPercent(CurrentPlayer->GetHardAttackRatio());
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