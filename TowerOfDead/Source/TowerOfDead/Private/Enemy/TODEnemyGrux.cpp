#include "Enemy/TODEnemyGrux.h"
#include "Enemy/TODEnemyGruxAIController.h"
#include "Enemy/TODAIAnimInstance.h"

ATODEnemyGrux::ATODEnemyGrux()
{
	AIControllerClass = ATODEnemyGruxAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	IsCanDashAttack = true;
}

void ATODEnemyGrux::Attack()
{
	if (!GetIsCanAttack())
		return;

	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	AnimInstance->PlayAttackMontage();
	SetIsCanAttack(false);
}

bool ATODEnemyGrux::OutRangeAttack()
{
	// 어떤 기술을 사용할 것인지? & 해당 기술은 사용 가능한 상태인지

	// 1순위 - 거리가 멀다면 대쉬 기술

	return false;
}