#pragma once

#include "TowerOfDead.h"
#include "TODCharacter.h"
#include "TODKwangCharacter.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODKwangCharacter : public ATODCharacter
{
	GENERATED_BODY()
	
public:
	ATODKwangCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	float GetCastSkillRatio();

	void Attack();
	void AttackStartComboState();
	void AttackEndComboState();
	bool HitEnemyCheck(class ATODEnemy* enemy);

	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// 차징 스킬
	void ActionMouseRight();
	void ActionMouseRightEnd();

	void HardAttackEnd();
	void HardAttackCoolDownTimer();

	// 무기 투척
	void ActionKeyboardR();
	void ActionKeyboardREnd();

	/*
	void SpecialAttack();
	void SpecialAttackCatch();
	void SpecialAttackEnd();
	void SpecialAttackCatchTimer();
	void SpecialAttackCoolDownTimer();
	*/

private:
	// 공격 1번에 1번만 Damage 받을 수 있도록
	UPROPERTY()
	TArray<ATODEnemy*> ArrHitEnemyCheck;

	// 콤보 공격 ------------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//bool CanAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
	//-------------------------------------------------------

	// 강공격 --------------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsHardAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanHardAttack;

	float CastTime = 0.0f;
	float HardAttackTime = 2.0f;
	float HardAttackCoolDownTime = 5.0f;
	FTimerHandle HardAttackTimerHandle;
	// -----------------------------------------------------

	// 특수 공격 --------------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsSpecialAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanSpecialAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanSpecialCatch;

	float SpecialAttackCoolDownTime = 5.0f;

	FTimerHandle SpecialAttackTimerHandle;
	FTimerHandle SpecialCatchTimerHandle;

	bool IsWeaponFall = false;
	float FalldeltaTime = 0.0f;
	// -------------------------------------------------------
};
