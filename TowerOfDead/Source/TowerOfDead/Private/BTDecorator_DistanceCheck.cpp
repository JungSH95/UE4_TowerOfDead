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

	// �Ÿ� ��� �־����� True, ���� ���� ����� false
	float dis = Target->GetDistanceTo(ControllingPawn);

	// ���� �� (�ӽ� �Ÿ� 400.0f)
	if (dis <= 400.0f)
		return false;
	// ���� ���� ��
	else
		return true;
}