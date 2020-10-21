#include "Enemy/TODEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATODEnemyAIController::TargetKey(TEXT("Target"));
const FName ATODEnemyAIController::AIStateKey(TEXT("State"));
const FName ATODEnemyAIController::IsAttackingKey(TEXT("IsAttacking"));
const FName ATODEnemyAIController::IsCanOutRangeAttackKey(TEXT("IsCanOutRangeAttack"));
const FName ATODEnemyAIController::RandomPosKey(TEXT("RandomPos"));

ATODEnemyAIController::ATODEnemyAIController()
{
	bSetControlRotationFromPawnOrientation = false;
}

void ATODEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void ATODEnemyAIController::SetIsAttaking(bool isattack)
{
	Blackboard->SetValueAsBool(IsAttackingKey, isattack);
}

void ATODEnemyAIController::SetIsDead()
{
	Blackboard->SetValueAsEnum(ATODEnemyAIController::AIStateKey, (uint8)EnemyState::DEAD);
}

void ATODEnemyAIController::StartAI()
{
	if (BTAsset != nullptr)
	{
		if (!RunBehaviorTree(BTAsset))
			TODLOG(Error, TEXT("AIController not Run BT"));
	}
}