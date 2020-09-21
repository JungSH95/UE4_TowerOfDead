#include "BTDecorator_DistanceCheck.h"
#include "TODEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_DistanceCheck::UBTDecorator_DistanceCheck()
{
	NodeName = TEXT("DistanceCheck");
}

bool UBTDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return true;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATODEnemyAIController::TargetKey));
	if (Target == nullptr)
		return true;

	// 거리 계산 멀어지면 True, 일정 범위 내라면 false
	float dis = Target->GetDistanceTo(ControllingPawn);

	// 범위 내 (임시 거리 400.0f)
	if (dis <= 400.0f)
		return false;
	// 일정 범위 밖
	else
		return true;
}