#include "Enemy/TODEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATODEnemyAIController::TargetKey(TEXT("Target"));
const FName ATODEnemyAIController::AIStateKey(TEXT("State"));
const FName ATODEnemyAIController::IsAttackingKey(TEXT("IsAttacking"));
const FName ATODEnemyAIController::IsCanOutRangeAttackKey(TEXT("IsCanOutRangeAttack"));
const FName ATODEnemyAIController::RandomAttackCheckKey(TEXT("RandomAttackCheck"));
const FName ATODEnemyAIController::RandomPosKey(TEXT("RandomPos"));

ATODEnemyAIController::ATODEnemyAIController()
{
	bSetControlRotationFromPawnOrientation = false;
}

void ATODEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void ATODEnemyAIController::SetIsAttaking(bool isAttack)
{
	Blackboard->SetValueAsBool(IsAttackingKey, isAttack);
}

void ATODEnemyAIController::SetIsCanOutRangeAttack(bool isCanOutRangeAttack)
{
	Blackboard->SetValueAsBool(IsCanOutRangeAttackKey, isCanOutRangeAttack);
}

void ATODEnemyAIController::SetIsDead()
{
	Blackboard->SetValueAsEnum(ATODEnemyAIController::AIStateKey, (uint8)EnemyState::DEAD);
}