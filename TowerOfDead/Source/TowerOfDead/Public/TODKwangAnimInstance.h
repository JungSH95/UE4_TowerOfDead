#pragma once

#include "TowerOfDead.h"
#include "TODAnimInstance.h"
#include "TODKwangAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHardAttackHitCheckElegate, int32, float);
DECLARE_MULTICAST_DELEGATE(FOnHardAttackEndDelegate);

UCLASS()
class TOWEROFDEAD_API UTODKwangAnimInstance : public UTODAnimInstance
{
	GENERATED_BODY()
	
public:
	UTODKwangAnimInstance();

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	void PlayHardAttackMontage();
	void PlayThrowMontage();
	void PlayCatchMontage();

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnHardAttackHitCheckElegate OnHardAttackHitCheck;
	FOnHardAttackEndDelegate OnHardAttackEnd;

	UAnimMontage* GetHardAttackMontage() { return HardAttackMontage; }
	UAnimMontage* GetSpecialAttackThrowMontage() { return SpecialAttackThrowMontage; }

	void SetTargetPoint(FVector pos) { TargetPoint = pos; }
	FVector GetTargetPoint() { return TargetPoint; }
	bool GetIsSpecialTarget() { return IsSpecialTarget; }
	void SetSpecialAttacking(bool isSpecialAttacking) { IsSpecialAttacking = isSpecialAttacking; }

private:
	FName GetAttackMontageSectionName(int32 Section);

	UFUNCTION()
	void AnimNotify_SetCanAttack();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();
	UFUNCTION()
	void AnimNotify_AttackHitCheckStart();
	UFUNCTION()
	void AnimNotify_AttackHitCheckEnd();
	
	UFUNCTION()
	void AnimNotify_HardAttackStart();
	UFUNCTION()
	void AnimNotify_HardAttackEnd();
	UFUNCTION()
	void AnimNotify_HardAttackHitCheck();

	UFUNCTION()
	void AnimNotify_SpecialTargeting();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HardAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SpecialAttackThrowMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SpecialAttackCatchMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsSpecialAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsSpecialTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FVector TargetPoint;
};
