#include "TODLevelUpWidget.h"
#include "TODPlayerState.h"
#include "TODCharacterStatComponent.h"
#include "TODGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UTODLevelUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NowTotalLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowTotalLevel_Text")));
	NextTotalLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NextTotalLevel_Text")));

	NowSoul = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowSoul_Text")));
	NextSoul = Cast<UTextBlock>(GetWidgetFromName(TEXT("NextSoul_Text")));
	NeedSoul = Cast<UTextBlock>(GetWidgetFromName(TEXT("NeedSoul_Text")));

	NowHPLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowHPLevel_Text")));
	NextHPLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NextHPLevel_Text")));
	NowATKLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowATKLevel_Text")));
	NextATKLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NextATKLevel_Text")));
	NowDEFLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowDEFLevel_Text")));
	NextDEFLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("NextDEFLevel_Text")));

	NowHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowHP_Text")));
	NowATK = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowATK_Text")));
	NowDEF = Cast<UTextBlock>(GetWidgetFromName(TEXT("NowDEF_Text")));

	HP_DownButton = Cast<UButton>(GetWidgetFromName(TEXT("HPDownBtn")));
	if (HP_DownButton != nullptr)
		HP_DownButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnHPDownClicked);
	HP_UpButton = Cast<UButton>(GetWidgetFromName(TEXT("HPUpBtn")));
	if (HP_UpButton != nullptr)
		HP_UpButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnHPUpClicked);

	ATK_DownButton = Cast<UButton>(GetWidgetFromName(TEXT("ATKDownBtn")));
	if (ATK_DownButton != nullptr)
		ATK_DownButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnATKDownClicked);
	ATK_UpButton = Cast<UButton>(GetWidgetFromName(TEXT("ATKUpBtn")));
	if (ATK_UpButton != nullptr)
		ATK_UpButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnATKUpClicked);

	DEF_DownButton = Cast<UButton>(GetWidgetFromName(TEXT("DEFDownBtn")));
	if (DEF_DownButton != nullptr)
		DEF_DownButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnDEFDownClicked);
	DEF_UpButton = Cast<UButton>(GetWidgetFromName(TEXT("DEFUpBtn")));
	if (DEF_UpButton != nullptr)
		DEF_UpButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnDEFUpClicked);

	Action_Button = Cast<UButton>(GetWidgetFromName(TEXT("ActionBtn")));
	if (Action_Button != nullptr)
		Action_Button->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnActionClicked);
}

void UTODLevelUpWidget::BindPlayerStateClass(class ATODPlayerState* playerState)
{
	if (playerState != nullptr)
	{
		CurrentPlayerState = playerState;
		UpdateNowPlayerState();
		UpdateNextPlayerState();
	}
}

void UTODLevelUpWidget::BindCharacterStatClass(class UTODCharacterStatComponent* characterStat)
{
	if (characterStat != nullptr)
		CurrentCharacterStat = characterStat;
}

void UTODLevelUpWidget::UpdateNowPlayerState()
{
	if (CurrentPlayerState == nullptr)
		return;
	
	// ���� Player�� �ɷ�ġ
	if (NowTotalLevel != nullptr)
		NowTotalLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetTotalLevel())));
	if(NowSoul != nullptr)
		NowSoul->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCurrentSoul())));
	if(NowHPLevel != nullptr) 
		NowHPLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetHPLevel())));
	if(NowATKLevel != nullptr)
		NowATKLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetATKLevel())));
	if(NowDEFLevel != nullptr)
		NowDEFLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetDEFLevel())));
}

void UTODLevelUpWidget::UpdateNextPlayerState()
{
	if (CurrentPlayerState == nullptr)
		return;

	// Button Ŭ�� �� ��ȭ�Ǵ� ����
	if (NextTotalLevel != nullptr)
		NextTotalLevel->SetText(FText::FromString(FString::FromInt(
			CurrentPlayerState->GetTotalLevel() + HPCount + ATKCount + DEFCount)));	
	if (NeedSoul != nullptr)
	{
		TODGameInstance = Cast<UTODGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		if (TODGameInstance != nullptr)
		{
			int32 totalneedSoul = 0;
			
			// HP ���� �� �ʿ��� �ҿ�
			for (int i = 0; i < HPCount; i++)
				totalneedSoul += TODGameInstance->GetTODCharacterData(CurrentPlayerState->GetHPLevel() + i)->NextSoul;

			// ATK ���� �� �ʿ��� �ҿ�
			for (int i = 0; i < ATKCount; i++)
				totalneedSoul += TODGameInstance->GetTODCharacterData(CurrentPlayerState->GetATKLevel() + i)->NextSoul;

			// DEF ���� �� �ʿ��� �ҿ�
			for (int i = 0; i < DEFCount; i++)
				totalneedSoul += TODGameInstance->GetTODCharacterData(CurrentPlayerState->GetDEFLevel() + i)->NextSoul;

			TotalNeedSoul = totalneedSoul;
			NeedSoul->SetText(FText::FromString(FString::FromInt(totalneedSoul)));

			if (NowHP != nullptr)
				NowHP->SetText(FText::FromString(FString::FromInt(TODGameInstance->GetTODCharacterData(CurrentPlayerState->GetHPLevel() + HPCount)->MaxHp)));;
			if (NowATK != nullptr)
				NowATK->SetText(FText::FromString(FString::FromInt(TODGameInstance->GetTODCharacterData(CurrentPlayerState->GetATKLevel() + ATKCount)->Attack)));;
			if (NowDEF != nullptr)
				NowDEF->SetText(FText::FromString(FString::FromInt(TODGameInstance->GetTODCharacterData(CurrentPlayerState->GetDEFLevel() + DEFCount)->Defense)));;
		}
	}
	if (NextSoul != nullptr)
		NextSoul->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCurrentSoul() - TotalNeedSoul)));
	
	if (NextHPLevel != nullptr)
		NextHPLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetHPLevel() + HPCount)));
	if (NextATKLevel != nullptr)
		NextATKLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetATKLevel() + ATKCount)));
	if (NextDEFLevel != nullptr)
		NextDEFLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetDEFLevel() + DEFCount)));
}

void UTODLevelUpWidget::OnHPDownClicked()
{
	print(FString::Printf(TEXT("HP Down Button Click")));

	HPCount--;
	if (HPCount <= 0)
		HPCount = 0;

	UpdateNextPlayerState();
}

void UTODLevelUpWidget::OnHPUpClicked()
{
	print(FString::Printf(TEXT("HP Up Button Click")));

	HPCount++;
	// ���� ���� ������ �� 
	if (CurrentPlayerState != nullptr)
		if (CurrentPlayerState->GetHPLevel() + HPCount > 10)
			HPCount--;

	UpdateNextPlayerState();
}


void UTODLevelUpWidget::OnATKDownClicked()
{
	print(FString::Printf(TEXT("ATK Down Button Click")));

	ATKCount--;
	if (ATKCount <= 0)
		ATKCount = 0;

	UpdateNextPlayerState();
}

void UTODLevelUpWidget::OnATKUpClicked()
{
	print(FString::Printf(TEXT("ATK Up Button Click")));

	ATKCount++;
	// ���� ���� ������ �� 
	if (CurrentPlayerState != nullptr)
		if (CurrentPlayerState->GetATKLevel() + ATKCount > 10)
			ATKCount--;

	UpdateNextPlayerState();
}

void UTODLevelUpWidget::OnDEFDownClicked()
{
	print(FString::Printf(TEXT("DEF Down Button Click")));

	DEFCount--;
	if (DEFCount <= 0)
		DEFCount = 0;

	UpdateNextPlayerState();
}

void UTODLevelUpWidget::OnDEFUpClicked()
{
	print(FString::Printf(TEXT("DEF Up Button Click")));

	DEFCount++;
	// ���� ���� ������ �� 
	if (CurrentPlayerState != nullptr)
		if (CurrentPlayerState->GetDEFLevel() + DEFCount > 10)
			DEFCount--;

	UpdateNextPlayerState();
}

void UTODLevelUpWidget::OnActionClicked()
{
	print(FString::Printf(TEXT("Action Button Click")));

	// ������ �ƴٸ� ���� Player State ���� �� Ŭ���� ��ư �� �ʱ�ȭ
	
	//UpdateNowPlayerState();
	//UpdateNextPlayerState();
}