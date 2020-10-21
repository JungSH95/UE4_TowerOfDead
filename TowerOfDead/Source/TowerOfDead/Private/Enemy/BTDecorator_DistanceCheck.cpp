#include "Enemy/BTDecorator_DistanceCheck.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
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

	// ���� �� (�ӽ� �Ÿ� 400.0f)
	if (dis <= Enemy->GetEffectiveRange())
	{
		// ������ �����ϴٸ� CanAttack���� true�� ���� �� Player ����
		Enemy->GetCharacterMovement()->bOrientRotationToMovement = false;
		return false;
	}

	// ���� ���� �� (������ ������Ƿ� Player ����)
	Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;
	return true;
}