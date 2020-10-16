#include "TODUserWidget.h"
#include "TODCharacter.h"
#include "TODCharacterStatComponent.h"
#include "TODPlayerState.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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
	CharacterName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName_Text")));
	CurrentSoul = Cast<UTextBlock>(GetWidgetFromName(TEXT("Soul_Text")));
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
		characterStat->OnHPChanged.AddUObject(this, &UTODUserWidget::UpdateCharacterStat);
	}
}

void UTODUserWidget::BindPlayerStateClass(class ATODPlayerState* playerState)
{
	if (playerState != nullptr)
	{
		CurrentPlayerState = playerState;
		playerState->OnPlayerStateChange.AddUObject(this, &UTODUserWidget::UpdatePlayerState);
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

	if (CharacterName != nullptr)
		CharacterName->SetText(FText::FromString("Kwang"));
	if (CurrentSoul != nullptr)
		CurrentSoul->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCurrentSoul())));
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