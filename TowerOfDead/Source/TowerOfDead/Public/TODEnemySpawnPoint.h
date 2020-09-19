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
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	int EnemyNumber;
};
