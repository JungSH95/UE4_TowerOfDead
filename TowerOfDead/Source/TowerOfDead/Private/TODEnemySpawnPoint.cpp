#include "TODEnemySpawnPoint.h"

ATODEnemySpawnPoint::ATODEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Tags.Add("EnemySpawnPoint");
	EnemyNumber = -1;
}

void ATODEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATODEnemySpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

