#include "TODDrongoAnimInstance.h"

UTODDrongoAnimInstance::UTODDrongoAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Primary_Fire_Montage.Primary_Fire_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;
}

void UTODDrongoAnimInstance::PlayAttackMontage()
{
	if (AttackMontage != nullptr)
	{
		Montage_Play(AttackMontage, 1.0f);

		print(FString::Printf(TEXT("Play Drongo Attack Montage")));
	}
}