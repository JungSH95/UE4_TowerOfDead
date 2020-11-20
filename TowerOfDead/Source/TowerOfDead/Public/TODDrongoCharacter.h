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

	void SetIsCanAttack(bool attack) { IsCanAttack = attack; }

	void Attack();
	void AttackDelayTime();
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void ActionMouseRight();
	void ActionMouseRightEnd();

	void ActionKeyboardR();
	void ActionKeyboardREnd();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FTimerHandle AttackDelayTimerHandle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsBazookaAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsGrenadeAttacking;
};
