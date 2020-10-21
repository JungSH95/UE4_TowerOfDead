#pragma once

#include "TowerOfDead.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsCanAttack.generated.h"

UCLASS()
class TOWEROFDEAD_API UBTDecorator_IsCanAttack : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsCanAttack();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory) const override;
};
