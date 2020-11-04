#include "TODLevelUpWidget.h"
#include "Components/Button.h"

void UTODLevelUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HP_DownButton = Cast<UButton>(GetWidgetFromName(TEXT("HPDown_Button")));
	if (HP_DownButton != nullptr)
		HP_DownButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnHPDownClicked);
	HP_UpButton = Cast<UButton>(GetWidgetFromName(TEXT("HPUp_Button")));
	if (HP_UpButton != nullptr)
		HP_UpButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnHPUpClicked);

	ATK_DownButton = Cast<UButton>(GetWidgetFromName(TEXT("ATKDown_Button")));
	if (ATK_DownButton != nullptr)
		ATK_DownButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnATKDownClicked);
	ATK_UpButton = Cast<UButton>(GetWidgetFromName(TEXT("ATKUp_Button")));
	if (ATK_UpButton != nullptr)
		ATK_UpButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnATKUpClicked);

	DEF_DownButton = Cast<UButton>(GetWidgetFromName(TEXT("DEFDown_Button")));
	if (DEF_DownButton != nullptr)
		DEF_DownButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnDEFDownClicked);
	DEF_UpButton = Cast<UButton>(GetWidgetFromName(TEXT("DEFUp_Button")));
	if (DEF_UpButton != nullptr)
		DEF_UpButton->OnClicked.AddDynamic(this, &UTODLevelUpWidget::OnDEFUpClicked);
}

void UTODLevelUpWidget::OnHPDownClicked()
{
	print(FString::Printf(TEXT("HP Down Button Click")));
}

void UTODLevelUpWidget::OnHPUpClicked()
{
	print(FString::Printf(TEXT("HP Up Button Click")));
}


void UTODLevelUpWidget::OnATKDownClicked()
{
	print(FString::Printf(TEXT("ATK Down Button Click")));
}

void UTODLevelUpWidget::OnATKUpClicked()
{
	print(FString::Printf(TEXT("ATK Up Button Click")));
}

void UTODLevelUpWidget::OnDEFDownClicked()
{
	print(FString::Printf(TEXT("DEF Down Button Click")));
}

void UTODLevelUpWidget::OnDEFUpClicked()
{
	print(FString::Printf(TEXT("DEF Up Button Click")));
}
