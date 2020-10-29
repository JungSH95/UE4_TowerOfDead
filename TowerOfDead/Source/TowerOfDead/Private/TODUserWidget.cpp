#include "TODUserWidget.h"
#include "TODCharacter.h"
#include "TODCharacterStatComponent.h"
#include "TODPlayerState.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "TODGameMode.h"
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

	CurrentStage = Cast<UTextBlock>(GetWidgetFromName(TEXT("StageInfo_Text")));
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Player_HPBar")));
	CharacterName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName_Text")));
	CurrentSoul = Cast<UTextBlock>(GetWidgetFromName(TEXT("Soul_Text")));

	BossHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Boss_HPBar")));
	BossName = Cast<UTextBlock>(GetWidgetFromName(TEXT("BossName_Text")));
	SetBossInfoVisible(false);
}

void UTODUserWidget::BindGameModeClass(class ATODGameMode* gamemode)
{
	if (gamemode != nullptr)
	{
		CurrentGameMode = gamemode;
		CurrentGameMode->OnNextStage.AddUObject(this, &UTODUserWidget::UpdateGameMode);
		UpdateGameMode();
	}
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

void UTODUserWidget::BindBossEnemyStateClass(class UTODEnemyStatComponent* enemyStat)
{
	if (enemyStat != nullptr)
	{
		BossEnemyStat = enemyStat;
		BossEnemyStat->OnHPChanged.AddUObject(this, &UTODUserWidget::UpdateBossState);
		UpdateBossState();
		SetBossInfoVisible(true);
	}
	else
		SetBossInfoVisible(false);
}

void UTODUserWidget::UpdateGameMode()
{
	if (CurrentGameMode == nullptr)
		return;

	if (CurrentStage != nullptr)
		CurrentStage->SetText(FText::FromString(CurrentGameMode->GetStageInfo()));
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

void UTODUserWidget::UpdateBossState()
{
	if (BossEnemyStat == nullptr)
		return;

	if (BossName != nullptr)
		BossName->SetText(FText::FromString(BossEnemyStat->GetName()));
	if (BossHPBar != nullptr)
	{
		BossHPBar->SetPercent(BossEnemyStat->GetHPRatio());
		if (BossEnemyStat->GetHPRatio() <= 0.0f)
			SetBossInfoVisible(false);
	}
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

void UTODUserWidget::SetBossInfoVisible(bool isVisible)
{
	if (BossName == nullptr || BossHPBar == nullptr)
		return;

	if (isVisible)
	{
		BossName->SetVisibility(ESlateVisibility::Visible);
		BossHPBar->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		BossName->SetVisibility(ESlateVisibility::Hidden);
		BossHPBar->SetVisibility(ESlateVisibility::Hidden);
	}
}

