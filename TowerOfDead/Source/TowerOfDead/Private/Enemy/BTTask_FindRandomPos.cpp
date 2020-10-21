#include "Enemy/BTTask_FindRandomPos.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomPos::UBTTask_FindRandomPos()
{
	NodeName = TEXT("FindRandomPos");
}

EBTNodeResult::Type UBTTask_FindRandomPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ATODEnemy* Enemy = Cast<ATODEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATODEnemyAIController::TargetKey));
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	// Enemy�� ��ġ Rotation���� -45 ~ +45
	FVector LookVector = Enemy->GetActorLocation() - Target->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	FRotator RandomRot = FRotator::ZeroRotator;
	RandomRot.Yaw = FMath::FRandRange(TargetRot.Yaw - 45.0f, TargetRot.Yaw + 45.0f);
	
	// Player�� �߽����� Enemy�� EffectiveRange ���� ������ �̵�
	float NewEffectiveRange = FMath::FRandRange(Enemy->GetEffectiveRange() - 100.0f, Enemy->GetEffectiveRange());

	// Player �ֺ��� Ư�� ��ġ
	FVector NewRandomPos = RandomRot.RotateVector(FVector(1.0f, 0.0f, 0.0f))
		* NewEffectiveRange + Target->GetActorLocation();

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ATODEnemyAIController::RandomPosKey, NewRandomPos);

	return EBTNodeResult::Succeeded;
}