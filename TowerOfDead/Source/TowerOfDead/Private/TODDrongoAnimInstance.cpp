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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BAZOOKAFIRE_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Drongo_BazookaFire_Montage.Drongo_BazookaFire_Montage"));
	if (BAZOOKAFIRE_MONTAGE.Succeeded())
		BazookaFireMontage = BAZOOKAFIRE_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BAZOOKALAND_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Drongo_BazookaLand_Montage.Drongo_BazookaLand_Montage"));
	if (BAZOOKALAND_MONTAGE.Succeeded())
		BazookaLandMontage = BAZOOKALAND_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GRENADEPREP_MONTAGE(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Animations/Drongo_GrenadePrep_Montage.Drongo_GrenadePrep_Montage"));
	if (GRENADEPREP_MONTAGE.Succeeded())
		GrenadePrepMontage = GRENADEPREP_MONTAGE.Object;

	IsBazooka = false;
	IsBazookaFire = false;

	IsGrenade = false;
}

void UTODDrongoAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 발사하고, 땅에 착지했다면
	if (IsBazookaFire && !IsAir)
	{
		IsBazookaFire = false;
		PlayBazookLandMontage();
	}
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

void UTODDrongoAnimInstance::PlayBazookFireMontage()
{
	if (BazookaFireMontage != nullptr)
	{
		Montage_Play(BazookaFireMontage, 1.0f);
		IsBazookaFire = true;
	}
}

void UTODDrongoAnimInstance::PlayBazookLandMontage()
{
	if (BazookaLandMontage != nullptr)
	{
		Montage_Play(BazookaLandMontage, 1.0f);
		IsBazooka = false;
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