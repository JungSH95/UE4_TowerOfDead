#pragma once

#include "TowerOfDead.h"
#include "TODEnemyAIController.h"
#include "TODEnemyMeleeAIController.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemyMeleeAIController : public ATODEnemyAIController
{
	GENERATED_BODY()
	
public:
	ATODEnemyMeleeAIController();

	virtual void OnPossess(APawn* InPawn) override;

public:
	void StartAI();

};
