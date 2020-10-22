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

	static const FName IsAttackingKey;
	static const FName IsCanOutRangeAttackKey;

	static const FName RandomPosKey;

	void SetIsAttaking(bool isAttack);
	void SetIsCanOutRangeAttack(bool isCanOutRangeAttack);
	void SetIsDead();

	void StartAI();

protected:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;
};
