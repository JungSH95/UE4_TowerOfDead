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

void UTODEnemyStatComponent::SetDamage(float NewDamage)
{
	if (CurrentStatData == nullptr)
		return;

	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHp);

	if (CurrentHP <= 0.0f)
		OnHPIsZero.Broadcast();
}

float UTODEnemyStatComponent::GetAttack()
{
	float Damage = 0.0f;

	if (CurrentStatData != nullptr)
		Damage = CurrentStatData->Attack;

	return Damage;
}