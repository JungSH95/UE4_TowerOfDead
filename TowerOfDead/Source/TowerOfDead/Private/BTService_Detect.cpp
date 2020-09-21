#include "BTService_Detect.h"
#include "TODCharacter.h"
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
	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(ATODEnemyAIController::AIState,
		(uint8)EnemyState::CHASE);

	/*
	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AABCharacter* ABCharacter = Cast<AABCharacter>(OverlapResult.GetActor());
			if (ABCharacter && ABCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AABAIController::TargetKey, ABCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, ABCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), ABCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}
	*/
}