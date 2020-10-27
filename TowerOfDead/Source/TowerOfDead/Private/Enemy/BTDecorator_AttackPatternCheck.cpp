#include "Enemy/BTDecorator_AttackPatternCheck.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_AttackPatternCheck::UBTDecorator_AttackPatternCheck()
{
	NodeName = TEXT("AttackPatternCheck");
}

bool UBTDecorator_AttackPatternCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	bool Result = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ATODEnemy* Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return false;

	// ���Ÿ� ���� or ����� ����
	if (Enemy->GetIsCanOutRangeAttack())
	{
		// ���� Ȯ��(�ӽ� : 30��)�� �Ϲ� ���� ���� ����
		float Rand = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ATODEnemyAIController::RandomAttackCheckKey);
		if (Rand == -1.0f)
		{
			Rand = FMath::FRandRange(0.0f, 1.0f);
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ATODEnemyAIController::RandomAttackCheckKey, Rand);

			print(FString::Printf(TEXT("Attack Pattern Ratio : %f"), Rand));
		}

		if (Rand >= 0.7f)
			return false;

		return true;
	}

	return false;
}