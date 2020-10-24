#include "Enemy/TODEnemyMeleeAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

ATODEnemyMeleeAIController::ATODEnemyMeleeAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/EnemyAI/BB_Enemy.BB_Enemy"));
	if (BBObject.Succeeded())
		BBAsset = BBObject.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/EnemyAI/BT_EnemyMelee.BT_EnemyMelee"));
	if (BTObject.Succeeded())
		BTAsset = BTObject.Object;
}

void ATODEnemyMeleeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsBool(IsAttackingKey, false);
		Blackboard->SetValueAsBool(IsCanOutRangeAttackKey, false);
	}
}

void ATODEnemyMeleeAIController::StartAI()
{
	if (BTAsset != nullptr)
	{
		if (!RunBehaviorTree(BTAsset))
			TODLOG(Error, TEXT("AIController not Run BT"));
	}
}