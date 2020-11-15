#include "TODKwangAnimInstance.h"
#include "TODCharacter.h"

UTODKwangAnimInstance::UTODKwangAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/PlayerComboAttack_Montage.PlayerComboAttack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HARDATTACK_MONTAGE(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/PlayerHardAttack_Montage.PlayerHardAttack_Montage"));
	if (HARDATTACK_MONTAGE.Succeeded())
		HardAttackMontage = HARDATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SPECIALATTACKTHROW_MONTAGE(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/Ability_Throw_Montage.Ability_Throw_Montage"));
	if (SPECIALATTACKTHROW_MONTAGE.Succeeded())
		SpecialAttackThrowMontage = SPECIALATTACKTHROW_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SPECIALATTACKCATCH_MONTAGE(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/Ability_Catch_Montage.Ability_Catch_Montage"));
	if (SPECIALATTACKCATCH_MONTAGE.Succeeded())
		SpecialAttackCatchMontage = SPECIALATTACKCATCH_MONTAGE.Object;

	IsSpecialAttacking = false;
	IsSpecialTarget = false;
}

void UTODKwangAnimInstance::PlayAttackMontage()
{
	if (AttackMontage != nullptr)
		Montage_Play(AttackMontage, 1.0f);
}

void UTODKwangAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (Montage_IsPlaying(AttackMontage))
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UTODKwangAnimInstance::PlayHardAttackMontage()
{
	if (HardAttackMontage != nullptr)
		Montage_Play(HardAttackMontage, 1.0f);
}

void UTODKwangAnimInstance::PlayThrowMontage()
{
	if (SpecialAttackThrowMontage != nullptr)
	{
		Montage_Play(SpecialAttackThrowMontage, 1.0f);

		IsEquip = false;
		IsSpecialAttacking = false;
	}
}

void UTODKwangAnimInstance::PlayCatchMontage()
{
	if (SpecialAttackCatchMontage != nullptr)
	{
		Montage_Play(SpecialAttackCatchMontage, 1.0f);
		IsSpecialTarget = false;
	}
}

FName UTODKwangAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, 4))
		return FName(*FString::Printf(TEXT("Attack%d"), Section));
	else
		return NAME_None;
}

void UTODKwangAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UTODKwangAnimInstance::AnimNotify_AttackHitCheckStart()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetCanAttackDamage(true);
	}
}

void UTODKwangAnimInstance::AnimNotify_AttackHitCheckEnd()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetCanAttackDamage(false);
	}
}

void UTODKwangAnimInstance::AnimNotify_HardAttackStart()
{
	Montage_SetPlayRate(HardAttackMontage, 0.3f);
}

void UTODKwangAnimInstance::AnimNotify_HardAttackEnd()
{
	OnHardAttackEnd.Broadcast();
}

void UTODKwangAnimInstance::AnimNotify_HardAttackHitCheck()
{
	OnHardAttackHitCheck.Broadcast(0, 400.0f);
}

void UTODKwangAnimInstance::AnimNotify_SpecialTargeting()
{
	IsSpecialTarget = true;
}