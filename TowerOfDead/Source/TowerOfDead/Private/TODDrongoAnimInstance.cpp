#include "TODDrongoAnimInstance.h"
#include "TODDrongoCharacter.h"

UTODDrongoAnimInstance::UTODDrongoAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Primary_Fire_Montage.Primary_Fire_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BAZOOKAEQUIP_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Drongo_BazookaEquip_Montage.Drongo_BazookaEquip_Montage"));
	if (BAZOOKAEQUIP_MONTAGE.Succeeded())
		BazookaEquipMontage = BAZOOKAEQUIP_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GRENADEPREP_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Drongo_GrenadePrep_Montage.Drongo_GrenadePrep_Montage"));
	if (GRENADEPREP_MONTAGE.Succeeded())
		GrenadePrepMontage = GRENADEPREP_MONTAGE.Object;

	IsBazooka = false;
	IsGrenade = false;
}

void UTODDrongoAnimInstance::PlayAttackMontage()
{
	if (AttackMontage != nullptr)
		Montage_Play(AttackMontage, 1.0f);
}

void UTODDrongoAnimInstance::PlayBazookzEquipMontage()
{
	if (BazookaEquipMontage != nullptr)
	{
		Montage_Play(BazookaEquipMontage, 1.0f);
		IsBazooka = true;
	}
}

void UTODDrongoAnimInstance::PlayGrenadePrepMontage()
{
	if (GrenadePrepMontage != nullptr)
	{
		Montage_Play(GrenadePrepMontage, 1.0f);
		IsGrenade = true;
	}
}
