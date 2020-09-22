#include "TODEnemy.h"
#include "TODEnemyAIController.h"

ATODEnemy::ATODEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ATODEnemyAIController::StaticClass();
	// ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� Enemy���� ANNSEnemyAIController���� ���� �� ���踦 �޴´�.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODEnemy"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	State = EnemyState::PEACE;
	AttackRange = 300.0f;
	EffectiveRange = AttackRange + 100.0f;
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

