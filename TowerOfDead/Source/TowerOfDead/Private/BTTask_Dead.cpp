#include "BTTask_Dead.h"
#include "TODEnemy.h"
#include "TODEnemyAIController.h"

UBTTask_Dead::UBTTask_Dead()
{
	NodeName = TEXT("Dead");
}

EBTNodeResult::Type UBTTask_Dead::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}