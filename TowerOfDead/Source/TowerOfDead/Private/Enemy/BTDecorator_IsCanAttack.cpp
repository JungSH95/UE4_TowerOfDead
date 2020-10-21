#include "Enemy/BTDecorator_IsCanAttack.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"

UBTDecorator_IsCanAttack::UBTDecorator_IsCanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsCanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ATODEnemy* Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return false;

	// ������ �����ϴٸ� ������� �̵� �� ���� 
	if (Enemy->GetIsCanAttack())
	{
		Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;
		return true;
	}

	return false;
}