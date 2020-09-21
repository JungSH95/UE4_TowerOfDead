#pragma once

#include "TowerOfDead.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetState.generated.h"

UCLASS()
class TOWEROFDEAD_API UBTTask_SetState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SetState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EnemyState NewState;
};
