#pragma once

#include "TowerOfDead.h"
#include "TODCharacter.h"
#include "TODDrongoCharacter.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODDrongoCharacter : public ATODCharacter
{
	GENERATED_BODY()

public:
	ATODDrongoCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

public:
	float GetSkillCastRatio();

	void Attack();
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void ActionMouseRight();
	void ActionMouseRightEnd();

	void ActionKeyboardR();
	void ActionKeyboardREnd();
};
