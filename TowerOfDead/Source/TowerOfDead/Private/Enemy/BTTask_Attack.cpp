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

	// Attack 테스크에 들어오는 경우
	// 1) 사정거리 내에 있고 공격을 할 수 있다면
	// 2) 사정거리 밖에 있고 IsCanOutRangeAttackKey 키가 true일 때

	// IsCanOutRangeAttackKey키가 true : 돌진기술/광역기술/소환기술 등이 존재
	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(ATODEnemyAIController::IsCanOutRangeAttackKey))
	{
		APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATODEnemyAIController::TargetKey));
		if (Target == nullptr)
			return EBTNodeResult::Succeeded;
		float dis = Target->GetDistanceTo(Enemy);
		print(FString::Printf(TEXT("Distance : %f"), dis));

		// 원거리 기술 사용 & 원거리 기술들 중 하나라도 사용 불가능 하다면 대상 추격
		if (!Enemy->OutRangeAttack(dis))
		{
			//OwnerComp.GetBlackboardComponent()->SetValueAsBool(ATODEnemyAIController::IsCanOutRangeAttackKey, false);
			return EBTNodeResult::Succeeded;
		}
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