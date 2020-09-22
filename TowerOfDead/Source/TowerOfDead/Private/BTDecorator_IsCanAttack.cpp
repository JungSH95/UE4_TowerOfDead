#include "BTDecorator_IsCanAttack.h"
#include "TODEnemy.h"
#include "TODEnemyAIController.h"

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

	// 공격이 가능하다면
	if (Enemy->GetIsCanAttack())
		return true;
	else
		Enemy->GetCharacterMovement()->bOrientRotationToMovement = false;

	return false;
}