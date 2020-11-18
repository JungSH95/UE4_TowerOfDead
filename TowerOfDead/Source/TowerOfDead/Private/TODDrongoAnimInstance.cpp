#include "TODDrongoAnimInstance.h"
#include "TODDrongoCharacter.h"

UTODDrongoAnimInstance::UTODDrongoAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Primary_Fire_Montage.Primary_Fire_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;

	IsBazooka = false;
	IsGrenade = false;
}

void UTODDrongoAnimInstance::PlayAttackMontage()
{
	if (AttackMontage != nullptr)
		Montage_Play(AttackMontage, 1.0f);
}

void UTODDrongoAnimInstance::AnimNotify_SetCanAttack()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODDrongoCharacter* Player = Cast<ATODDrongoCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetIsCanAttack(true);
	}
}
