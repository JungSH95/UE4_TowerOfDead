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

public:
	void PlayAttackMontage();
	bool PlayHitReactMontage(int dir);

	void PlayEnemySpawnCastMontage();
	void PlayMeteorCastMontage();

	bool IsAttackMontage(UAnimMontage* montage);
private:
	UFUNCTION()
	void AnimNotify_RandomDoubleAttack();

	UFUNCTION()
	void AnimNotify_DoubleAttackHitCheck();
	UFUNCTION()
	void AnimNotify_DoubleAttackHitCheckEnd();

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* LevelStartMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> ArrAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DoubleAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* EnemySpawnCastMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* MeteorCastMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDash;
};
