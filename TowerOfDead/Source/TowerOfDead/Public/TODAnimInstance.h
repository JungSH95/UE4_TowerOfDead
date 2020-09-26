#pragma once

#include "TowerOfDead.h"
#include "Animation/AnimInstance.h"
#include "TODAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
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
	void JumpToAttackMontageSection(int32 NewSection);

	void MoveMontage();

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnHardAttackEndDelegate OnHardAttackEnd;

	UAnimMontage* GetAttackMontage() { return AttackMontage; }
	UAnimMontage* GetHardAttackMontage() { return HardAttackMontage; }

private:
	UFUNCTION()
	void AnimNotify_LevelStartAnimEnd();
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

	UFUNCTION()
	void AnimNotify_HardAttackStart();
	UFUNCTION()
	void AnimNotify_HardAttackEnd();

private:
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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HardAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* SpecialAttackMoveMontage;
};
