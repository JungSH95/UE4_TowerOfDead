#pragma once

#include "TowerOfDead.h"
#include "Animation/AnimInstance.h"
#include "TODAIAnimInstance.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UTODAIAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	virtual void PlayAttackMontage() PURE_VIRTUAL(UTODAIAnimInstance::PlayAttackMontage, return;);
	virtual bool PlayHitReactMontage(int dir) PURE_VIRTUAL(UTODAIAnimInstance::PlayHitReactMontage, return false;);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack)
	UAnimMontage* NowMontage;

private:
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_AttackHitCheckEnd();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EnemyState State;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

};
