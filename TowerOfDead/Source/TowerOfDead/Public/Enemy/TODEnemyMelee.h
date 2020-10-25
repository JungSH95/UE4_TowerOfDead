#pragma once

#include "TowerOfDead.h"
#include "TODEnemy.h"
#include "TODEnemyMelee.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemyMelee : public ATODEnemy
{
	GENERATED_BODY()

public:
	ATODEnemyMelee();

	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void Attack();
	void StartHitEffect(FVector pos);
};
