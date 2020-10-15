#include "TODCharacterStatComponent.h"
#include "TODGameInstance.h"

UTODCharacterStatComponent::UTODCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}


void UTODCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UTODCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	//SetNewCharacterStat(1, 1, 1, GetWorld());
}

void UTODCharacterStatComponent::SetNewCharacterStat(int32 HpLevel, int32 AtkLevel, int32 DefLevel,
	UWorld* World)
{
	auto TODGameInstance = Cast<UTODGameInstance>(UGameplayStatics::GetGameInstance(World));
	if (TODGameInstance != nullptr)
	{
		FTODCharacterData* HpData = TODGameInstance->GetTODCharacterData(HpLevel);
		if (HpData != nullptr)
		{
			MaxHP = HpData->MaxHp;
			CurrentHP = MaxHP;

			OnHPChanged.Broadcast();
		}

		FTODCharacterData* AttackData = TODGameInstance->GetTODCharacterData(HpLevel);
		if(AttackData != nullptr)
			Attack = AttackData->Attack;

		FTODCharacterData* DefenseData = TODGameInstance->GetTODCharacterData(HpLevel);
		if (DefenseData != nullptr)
			Defense = DefenseData->Attack;
	}
	TODLOG_S(Warning);
}

void UTODCharacterStatComponent::SetDamage(float NewDamage)
{
	CurrentHP = FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, MaxHP);
	if (CurrentHP <= 0.0f)
		OnHPIsZero.Broadcast();
}