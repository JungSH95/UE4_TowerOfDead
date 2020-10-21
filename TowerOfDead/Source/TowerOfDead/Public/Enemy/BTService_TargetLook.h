#pragma once

#include "TowerOfDead.h"
#include "BehaviorTree/BTService.h"
#include "BTService_TargetLook.generated.h"

UCLASS()
class TOWEROFDEAD_API UBTService_TargetLook : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_TargetLook();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;

};
