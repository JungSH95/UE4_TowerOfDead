#pragma once

#include "TowerOfDead.h"
#include "Animation/AnimInstance.h"
#include "TODAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHardAttackHitCheckElegate, int32, float);
DECLARE_MULTICAST_DELEGATE(FOnHardAttackEndDelegate);

UCLASS()
class TOWEROFDEAD_API UTODAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTODAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void PlayHardAttackMontage();
	void PlayThrowMontage();
	void PlayCatchMontage();

	void JumpToAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnHardAttackHitCheckElegate OnHardAttackHitCheck;
	FOnHardAttackEndDelegate OnHardAttackEnd;

	UAnimMontage* GetAttackMontage() { return AttackMontage; }
	UAnimMontage* GetHardAttackMontage() { return HardAttackMontage; }
	UAnimMontage* GetSpecialAttackThrowMontage() { return SpecialAttackThrowMontage; }

	void SetSpecialAttacking(bool isSpecialAttacking) { IsSpecialAttacking = isSpecialAttacking; }
	bool GetIsSpecialTarget() {	return IsSpecialTarget; }

	void SetTargetPoint(FVector pos) { TargetPoint = pos; }
	FVector GetTargetPoint() { return TargetPoint; }

	void SetIsSoulRecovery(bool isSoulRecovery) { IsSoulRecovery = isSoulRecovery; }
	void SetIsDead() { IsDead = true; }

private:
	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	UFUNCTION()
	void AnimNotify_SetCanAttack();

	UFUNCTION()
	void AnimNotify_AttackHitCheckStart();
	UFUNCTION()
	void AnimNotify_AttackHitCheckEnd();

	FName GetAttackMontageSectionName(int32 Section);

	UFUNCTION()
	void AnimNotify_HardAttackStart();
	UFUNCTION()
	void AnimNotify_HardAttackEnd();
	UFUNCTION()
	void AnimNotify_HardAttackHitCheck();

	UFUNCTION()
	void AnimNotify_SpecialTargeting();

	UFUNCTION()
	void AnimNotify_SoulRecoveryLoopStart();

	UFUNCTION()
	void AnimNotify_SoulRecoveryEnd();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float AimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float AimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsEquip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsSpecialAttacking;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HardAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SpecialAttackThrowMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsSpecialTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FVector TargetPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SpecialAttackCatchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsSoulRecovery;
};
