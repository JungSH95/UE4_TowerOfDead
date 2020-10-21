#include "Enemy/TODGruxAIAnimInstance.h"

UTODGruxAIAnimInstance::UTODGruxAIAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_1(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_Attack1_Montage.Grux_Attack1_Montage"));
	if (ATTACK_MONTAGE_1.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_1.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_2(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_Attack2_Montage.Grux_Attack2_Montage"));
	if (ATTACK_MONTAGE_2.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_2.Object);
}

void UTODGruxAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UTODGruxAIAnimInstance::PlayAttackMontage()
{
	// 동작중인 몽타주가 없다면
	if (NowMontage == nullptr)
	{
		int MontageRandomNumber = FMath::RandRange(0, 1);
		NowMontage = ArrAttackMontage[MontageRandomNumber];
		Montage_Play(NowMontage, 0.8f);
	}
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
