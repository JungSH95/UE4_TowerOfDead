#include "BTService_TargetLook.h"
#include "TODEnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_TargetLook::UBTService_TargetLook()
{
	NodeName = TEXT("TargetLook");
}

void UBTService_TargetLook::TickNode(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;

	ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(ControllingPawn);
	if (EnemyAI == nullptr)
		return;

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ATODEnemyAIController::TargetKey));
	if (Target == nullptr)
		return;

	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));
}