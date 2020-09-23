#include "BTDecorator_DistanceCheck.h"
#include "TODEnemy.h"
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

	ATODEnemy* Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return true;

	// �Ÿ� ��� �־����� True, ���� ���� ����� false
	float dis = Target->GetDistanceTo(ControllingPawn);
	Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;

	// ���� �� (�ӽ� �Ÿ� 400.0f)
	if (dis <= Enemy->GetEffectiveRange())
		return false;
	// ���� ���� ��
	else
		return true;
}