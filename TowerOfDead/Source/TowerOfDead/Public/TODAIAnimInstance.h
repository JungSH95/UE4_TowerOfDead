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

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EnemyState State;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

};