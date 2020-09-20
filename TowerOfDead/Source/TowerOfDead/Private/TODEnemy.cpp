#include "TODEnemy.h"
#include "TODEnemyAIController.h"

ATODEnemy::ATODEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ATODEnemyAIController::StaticClass();
	// ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� Enemy���� ANNSEnemyAIController���� ���� �� ���踦 �޴´�.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	State = EnemyState::PEACE;
}

void ATODEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATODEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATODEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

