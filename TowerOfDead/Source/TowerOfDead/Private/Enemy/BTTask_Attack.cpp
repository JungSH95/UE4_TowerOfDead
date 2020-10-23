#include "Enemy/BTTask_Attack.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ATODEnemyAIController::IsAttackingKey, true);

	// Attack �׽�ũ�� ������ ���
	// 1) �����Ÿ� ���� �ְ� ������ �� �� �ִٸ�
	// 2) �����Ÿ� �ۿ� �ְ� IsCanOutRangeAttackKey Ű�� true�� ��

	// IsCanOutRangeAttackKeyŰ�� true : �������/�������/��ȯ��� ���� ����
	if(OwnerComp.GetBlackboardComponent()->GetValueAsBool(ATODEnemyAIController::IsCanOutRangeAttackKey))
	{
		// ���Ÿ� ����� �� �ϳ��� ��� �Ұ���(��� �߰�)
		if (!Enemy->OutRangeAttack())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ATODEnemyAIController::IsCanOutRangeAttackKey, false);
			return EBTNodeResult::Succeeded;
		}
		else
			Enemy->OutRangeAttack();
	}
	else
		Enemy->Attack();

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(ATODEnemyAIController::IsAttackingKey))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}