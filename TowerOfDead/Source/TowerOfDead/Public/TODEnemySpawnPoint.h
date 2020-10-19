#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Actor.h"
#include "TODEnemySpawnPoint.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemySpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ATODEnemySpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	int32 GetEnemyNumber() { return EnemyNumber; }
	int32 GetEnemyLevel() { return EnemyLevel; }
	bool GetIsBoss() { return IsBoss; }

private:
	UPROPERTY(EditInstanceOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
	int32 EnemyNumber;

	UPROPERTY(EditInstanceOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
	int32 EnemyLevel;

	UPROPERTY(EditInstanceOnly, Category = EnemyInfo, Meta = (AllowPrivateAccess = true))
	bool IsBoss;
};
