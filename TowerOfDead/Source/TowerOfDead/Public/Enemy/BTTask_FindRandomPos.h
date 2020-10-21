#pragma once

#include "TowerOfDead.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomPos.generated.h"

UCLASS()
class TOWEROFDEAD_API UBTTask_FindRandomPos : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_FindRandomPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;
};
