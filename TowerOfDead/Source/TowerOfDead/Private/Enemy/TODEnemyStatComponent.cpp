#include "Enemy/TODEnemyStatComponent.h"
#include "TODGameInstance.h"

UTODEnemyStatComponent::UTODEnemyStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	Name = "NoneName";
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

void UTODEnemyStatComponent::SetName(FString NewName)
{
	Name = NewName;
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
			SetHP(CurrentStatData->MaxHp);
		}
		else
			TODLOG(Error, TEXT("Level Data Not Find"));
	}
}

void UTODEnemyStatComponent::SetDamage(float NewDamage)
{
	if (CurrentStatData == nullptr)
		return;
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHp));
}

void UTODEnemyStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();

	if (CurrentHP <= 0.0f)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

FString UTODEnemyStatComponent::GetName()
{
	return Name;
}

int32 UTODEnemyStatComponent::GetLevel()
{
	return Level;
}

float UTODEnemyStatComponent::GetAttack()
{
	float Damage = 0.0f;

	if (CurrentStatData != nullptr)
		Damage = CurrentStatData->Attack;

	return Damage;
}

int32 UTODEnemyStatComponent::GetDropSoul()
{
	return CurrentStatData->DropSoul;
}

float UTODEnemyStatComponent::GetHPRatio()
{
	if (CurrentStatData == nullptr)
		return 0.0f;

	return CurrentHP / CurrentStatData->MaxHp; 
}