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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	void Attack();
	void StartHitEffect(FVector pos);
};
