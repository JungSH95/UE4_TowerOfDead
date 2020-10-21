#pragma once

#include "TowerOfDead.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_DistanceCheck.generated.h"

UCLASS()
class TOWEROFDEAD_API UBTDecorator_DistanceCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_DistanceCheck();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) const override;
};
