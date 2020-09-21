#include "BTTask_SetState.h"
#include "TODEnemy.h"
#include "TODEnemyAIController.h"
#include "BehaviorTree//BlackboardComponent.h"

UBTTask_SetState::UBTTask_SetState()
{
	NodeName = TEXT("SetState");
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ATODEnemyAIController::AIStateKey,
		(uint8)NewState);

	ATODEnemy* Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	Enemy->SetState(NewState);

	return EBTNodeResult::Succeeded;
}