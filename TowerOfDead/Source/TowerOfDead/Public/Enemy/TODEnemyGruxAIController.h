#pragma once

#include "TowerOfDead.h"
#include "Enemy/TODEnemyAIController.h"
#include "TODEnemyGruxAIController.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemyGruxAIController : public ATODEnemyAIController
{
	GENERATED_BODY()
	
public:
	ATODEnemyGruxAIController();

	virtual void OnPossess(APawn* InPawn) override;
};
