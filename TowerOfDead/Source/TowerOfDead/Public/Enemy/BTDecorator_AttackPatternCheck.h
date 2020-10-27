#pragma once

#include "TowerOfDead.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_AttackPatternCheck.generated.h"

UCLASS()
class TOWEROFDEAD_API UBTDecorator_AttackPatternCheck : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_AttackPatternCheck();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) const override;
};
