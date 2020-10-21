#include "Enemy/BTTask_SetState.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
#include "TODCharacter.h"
#include "BehaviorTree//BlackboardComponent.h"

UBTTask_SetState::UBTTask_SetState()
{
	NodeName = TEXT("SetState");
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	if (OwnerComp.GetBlackboardComponent()->GetValueAsEnum(ATODEnemyAIController::AIStateKey)
		!= (uint8)EnemyState::DEAD)
	{
		ATODCharacter* Target = Cast<ATODCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATODEnemyAIController::TargetKey));
		if (Target == nullptr)
			return EBTNodeResult::Failed;

		if (Target->GetIsDead())
		{
			ATODEnemy* Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
			Enemy->SetState(EnemyState::PEACE);

			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ATODEnemyAIController::AIStateKey,
				(uint8)EnemyState::PEACE);
		}
		else
		{
			ATODEnemy* Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
			Enemy->SetState(NewState);

			OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ATODEnemyAIController::AIStateKey,
				(uint8)NewState);
		}
	}

	return EBTNodeResult::Succeeded;
}