#include "BTTask_FindRandomPos.h"

UBTTask_FindRandomPos::UBTTask_FindRandomPos()
{
	NodeName = TEXT("FindRandomPos");
}

EBTNodeResult::Type UBTTask_FindRandomPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 1. 

	return EBTNodeResult::Succeeded;
}