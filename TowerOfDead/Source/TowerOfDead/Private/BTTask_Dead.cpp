#include "BTTask_Dead.h"

UBTTask_Dead::UBTTask_Dead()
{
	NodeName = TEXT("Dead");
}

EBTNodeResult::Type UBTTask_Dead::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}