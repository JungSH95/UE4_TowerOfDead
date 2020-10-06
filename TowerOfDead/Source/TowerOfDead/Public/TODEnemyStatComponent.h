#pragma once

#include "TowerOfDead.h"
#include "Components/ActorComponent.h"
#include "TODEnemyStatComponent.generated.h"


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

private:
	struct FTODEnemyData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
