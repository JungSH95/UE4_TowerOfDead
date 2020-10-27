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

	if (Enemy->GetIsCanAttack())
		Enemy->Attack();
	else
		return EBTNodeResult::Failed;
	
	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(ATODEnemyAIController::IsAttackingKey))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ATODEnemyAIController::RandomAttackCheckKey, -1.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}