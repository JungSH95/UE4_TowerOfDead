#pragma once

#include "TowerOfDead.h"
#include "TODAIAnimInstance.h"
#include "TODGruxAIAnimInstance.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODGruxAIAnimInstance : public UTODAIAnimInstance
{
	GENERATED_BODY()

public:
	UTODGruxAIAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	bool PlayHitReactMontage(int dir);

	void PlayDoubleAttackMontage();

private:
	UFUNCTION()
	void AnimNotify_RandomDoubleAttack();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> ArrAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DoubleAttackMontage;

	/*
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> ArrHitReactMontage;
	*/
};
