#include "Enemy/BTTask_OutRangeAttack.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_OutRangeAttack::UBTTask_OutRangeAttack()
{
	NodeName = TEXT("OutRangeAttack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_OutRangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ATODEnemyAIController::IsAttackingKey, true);

	// �Ÿ� ���ϱ�
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATODEnemyAIController::TargetKey));
	if (Target == nullptr)
		return EBTNodeResult::Failed;
	float dis = Target->GetDistanceTo(Enemy);

	// ���Ÿ� ���� or ��� ��� �Ǵ��� Enemy Class���� ����
	Enemy->OutRangeAttack(dis);

	return EBTNodeResult::InProgress;
}

void UBTTask_OutRangeAttack::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(ATODEnemyAIController::IsAttackingKey))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ATODEnemyAIController::RandomAttackCheckKey, -1.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}