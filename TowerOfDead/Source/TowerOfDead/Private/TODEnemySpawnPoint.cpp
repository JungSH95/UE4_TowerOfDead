#include "TODEnemySpawnPoint.h"

ATODEnemySpawnPoint::ATODEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add("EnemySpawnPoint");
	EnemyNumber = -1;
	EnemyLevel = 1;
}

void ATODEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}
