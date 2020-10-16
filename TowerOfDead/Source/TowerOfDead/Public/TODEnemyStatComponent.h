#pragma once

#include "TowerOfDead.h"
#include "Components/ActorComponent.h"
#include "TODEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWEROFDEAD_API UTODEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTODEnemyStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetNewLevel(int32 NewLevel);

	void SetDamage(float NewDamage);
	float GetAttack();

	int32 GetDropSoul();
	float GetHPRatio();

	FOnHPIsZeroDelegate OnHPIsZero;

private:
	struct FTODEnemyData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
