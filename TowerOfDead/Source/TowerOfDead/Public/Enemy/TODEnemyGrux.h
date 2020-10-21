#pragma once

#include "TowerOfDead.h"
#include "TODEnemy.h"
#include "TODEnemyGrux.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemyGrux : public ATODEnemy
{
	GENERATED_BODY()

public:
	ATODEnemyGrux();
	
	void Attack();
};
