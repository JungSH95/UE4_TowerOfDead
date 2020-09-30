#pragma once

#include "TowerOfDead.h"
#include "TODAIAnimInstance.h"
#include "TODMeleeAIAnimInstance.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODMeleeAIAnimInstance : public UTODAIAnimInstance
{
	GENERATED_BODY()
	
public:
	UTODMeleeAIAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	bool PlayHitReactMontage(int dir);

private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> ArrAttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TArray<UAnimMontage*> ArrHitReactMontage;
};
