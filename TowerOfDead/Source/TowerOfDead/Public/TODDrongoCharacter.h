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

	void SetIsCanAttack(bool attack);

	void Attack();
	void AttackDelayTime();
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void ActionMouseRight();
	void ActionMouseRightEnd();
	void GrenadeDelayTimer();

	void ActionKeyboardR();
	void ActionKeyboardREnd();
	void BazookaFire();
	void BazookaDelayTimer();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FTimerHandle AttackDelayTimerHandle;

	// Bazooka Action ------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanBazooka;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsBazookaAttacking;	// 바주카포 기술을 진행 중

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanBazookaFireAttack;	// 바주카포 발사 가능

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FTimerHandle BazookaDelayTimerHandle;
	//----------------------------------------------------

	// Grenade Action ------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanGrenade;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsGrenadeAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FTimerHandle GrenadeDelayTimerHandle;
	//----------------------------------------------------
};
