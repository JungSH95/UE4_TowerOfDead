#include "TODEnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ATODEnemyAIController::TargetKey(TEXT("Target"));
const FName ATODEnemyAIController::AIState(TEXT("State"));

ATODEnemyAIController::ATODEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/EnemyAI/BB_Enemy.BB_Enemy"));
	if (BBObject.Succeeded())
		BBAsset = BBObject.Object;

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/EnemyAI/BT_Enemy.BT_Enemy"));
	if (BTObject.Succeeded())
		BTAsset = BTObject.Object;

	bSetControlRotationFromPawnOrientation = false;
}

void ATODEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			TODLOG(Error, TEXT("AIController not Run BT"));
		}
		else
		{

		}
	}
}