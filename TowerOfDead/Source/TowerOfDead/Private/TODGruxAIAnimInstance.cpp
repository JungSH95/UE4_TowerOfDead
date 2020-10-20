#include "TODGruxAIAnimInstance.h"

UTODGruxAIAnimInstance::UTODGruxAIAnimInstance()
{
	
}

void UTODGruxAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UTODGruxAIAnimInstance::PlayAttackMontage()
{
	/*if (NowMontage == nullptr)
	{
		int MontageRandomNumber = FMath::RandRange(0, 1);
		NowMontage = ArrAttackMontage[MontageRandomNumber];
		Montage_Play(NowMontage, 0.8f);
	}*/
}

bool UTODGruxAIAnimInstance::PlayHitReactMontage(int dir)
{
	/*if (!Montage_IsPlaying(ArrHitReactMontage[dir]))
	{
		Montage_Play(ArrHitReactMontage[dir], 1.0f);
		return true;
	}*/
	TODLOG_S(Warning);

	return false;
}
