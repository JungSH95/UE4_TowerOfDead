#include "TODEnemyStatComponent.h"
#include "TODGameInstance.h"

UTODEnemyStatComponent::UTODEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Level = 1;
}

void UTODEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTODEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UTODEnemyStatComponent::SetNewLevel(int32 NewLevel)
{
	auto TODGameInstance = Cast<UTODGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (TODGameInstance != nullptr)
	{
		CurrentStatData = TODGameInstance->GetTODEnemyData(NewLevel);
		
		if (CurrentStatData != nullptr)
		{
			Level = CurrentStatData->Level;
			CurrentHP = CurrentStatData->MaxHp;
		}
		else
			TODLOG(Error, TEXT("Level Data Not Find"));
	}
}