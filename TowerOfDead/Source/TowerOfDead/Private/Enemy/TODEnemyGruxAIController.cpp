#include "Enemy/TODEnemyGruxAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ATODEnemyGruxAIController::ATODEnemyGruxAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/EnemyAI/BB_Enemy.BB_Enemy"));
	if (BBObject.Succeeded())
		BBAsset = BBObject.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/EnemyAI/BT_EnemyGrux.BT_EnemyGrux"));
	if (BTObject.Succeeded())
		BTAsset = BTObject.Object;
}

void ATODEnemyGruxAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TODLOG_S(Warning);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsBool(IsAttackingKey, false);
		Blackboard->SetValueAsBool(IsCanOutRangeAttackKey, true);
	}
}