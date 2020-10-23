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

	void DoubleAttackHitCheck();
	void DoubleAttackHitCheckEnd();

	bool OutRangeAttack();
	void StartHitEffect(FVector pos);

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* AttackHitEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* DoubleAttackHitEffect;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDoubleAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsCanDashAttack;
};
