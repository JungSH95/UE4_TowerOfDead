#include "TODMeleeAIAnimInstance.h"

UTODMeleeAIAnimInstance::UTODMeleeAIAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_1(TEXT("/Game/ParagonMinions/Characters/Minions/Down_Minions/Animations/Melee/Montage/Minion_Lane_Core_Attack1_Montage.Minion_Lane_Core_Attack1_Montage"));
	if (ATTACK_MONTAGE_1.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_1.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_2(TEXT("/Game/ParagonMinions/Characters/Minions/Down_Minions/Animations/Melee/Montage/Minion_Lane_Core_Attack2_Montage.Minion_Lane_Core_Attack2_Montage"));
	if (ATTACK_MONTAGE_2.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_2.Object);
}

void UTODMeleeAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UTODMeleeAIAnimInstance::PlayAttackMontage()
{
	if (NowMontage == nullptr)
	{
		int MontageRandomNumber = FMath::RandRange(0, 1);
		NowMontage = ArrAttackMontage[MontageRandomNumber];
		Montage_Play(NowMontage, 0.8f);
	}

	/*
	else if (!Montage_IsPlaying(NowMontage))
	{
		NowMontage = ArrAttackMontage[0];
		Montage_Play(NowMontage, 1.0f);
	}
	*/
}