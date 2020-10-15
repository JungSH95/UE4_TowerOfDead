#pragma once

#include "TowerOfDead.h"
#include "Components/ActorComponent.h"
#include "TODCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWEROFDEAD_API UTODCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTODCharacterStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	
public:
	void SetNewCharacterStat(int32 HpLevel, int32 AtkLevel, int32 DefLevel, UWorld* World);

	void SetDamage(float NewDamage);

	float GetAttack() { return Attack; }
	float GetHPRatio() { return CurrentHP / MaxHP; }

	FOnHPIsZeroDelegate OnHPIsZero;

private:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float Attack;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Defense;
};
