#include "TODEnemySpawnPoint.h"

ATODEnemySpawnPoint::ATODEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add("EnemySpawnPoint");
	Tags.Add("NowStageName");

	EnemyNumber = -1;
	EnemyLevel = 1;
	IsBoss = false;
}

void ATODEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}
