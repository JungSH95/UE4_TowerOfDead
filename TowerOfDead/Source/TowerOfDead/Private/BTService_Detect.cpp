#include "BTService_Detect.h"
#include "TODCharacter.h"
#include "TODEnemy.h"
#include "TODEnemyAIController.h"
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

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 6000.0f;
	
	TODLOG_S(Warning);
	
	ATODCharacter* player = Cast<ATODCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (player == nullptr)
		return;

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(ATODEnemyAIController::TargetKey,
		player);
}