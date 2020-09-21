#pragma once

#include "TowerOfDead.h"
#include "AIController.h"
#include "TODEnemyAIController.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATODEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;

	static const FName TargetKey;
	static const FName AIStateKey;

	static const FName IsChaseKey;
	static const FName IsOutRangeAttackKey;
	static const FName IsAttackKey;

	static const FName RandomPosKey;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
