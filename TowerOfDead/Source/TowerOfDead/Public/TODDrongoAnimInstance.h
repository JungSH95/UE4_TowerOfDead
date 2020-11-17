#pragma once

#include "TowerOfDead.h"
#include "TODAnimInstance.h"
#include "TODDrongoAnimInstance.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODDrongoAnimInstance : public UTODAnimInstance
{
	GENERATED_BODY()
	
public:
	UTODDrongoAnimInstance();

	void PlayAttackMontage();

private:
	UFUNCTION()
	void AnimNotify_SetCanAttack();
	
};
