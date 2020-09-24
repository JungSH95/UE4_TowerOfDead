#pragma once

#include "TowerOfDead.h"
#include "Animation/AnimInstance.h"
#include "TODAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);

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

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;

	UAnimMontage* GetAttackMontage() { return AttackMontage; }

private:
	UFUNCTION()
	void AnimNotify_LevelStartAnimEnd();
	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

	UFUNCTION()
	void AnimNotify_HardAttackStart();

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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HardAttackMontage;
};
