#pragma once

#include "TowerOfDead.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_OutRangeAttack.generated.h"

UCLASS()
class TOWEROFDEAD_API UBTTask_OutRangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_OutRangeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;
};
