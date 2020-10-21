#include "Enemy/BTService_Detect.h"
#include "TODCharacter.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
#include "BehaviorTree//BlackboardComponent.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;
		
	ATODCharacter* player = Cast<ATODCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player == nullptr)
		return;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATODEnemyAIController::TargetKey,
		player);
}