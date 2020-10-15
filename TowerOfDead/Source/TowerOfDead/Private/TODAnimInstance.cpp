#include "TODAnimInstance.h"
#include "TODPlayerController.h"
#include "Kismet/KismetMathLibrary.h"


UTODAnimInstance::UTODAnimInstance()
{
	CurrentSpeed = 0.0f;
	CurrentPawnDirection = 0.0f;
	AimPitch = 0.0f;
	AimYaw = 0.0f;

	IsAir = false;
	IsEquip = true;
	IsSpecialAttacking = false;
	IsSpecialTarget = false;

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
}

void UTODAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		IsAir = PlayerPawn->GetMovementComponent()->IsFalling();

		FRotator A = UKismetMathLibrary::MakeRotFromX(PlayerPawn->GetVelocity());
		CurrentPawnDirection = UKismetMathLibrary::NormalizedDeltaRotator(A, PlayerPawn->GetControlRotation()).Yaw;

		if (!IsAir)
			CurrentSpeed = PlayerPawn->GetVelocity().Size();
		else
			CurrentSpeed = 0.0f;
		
		FRotator AimRotator = UKismetMathLibrary::NormalizedDeltaRotator(PlayerPawn->GetBaseAimRotation(), PlayerPawn->GetActorRotation());
		AimPitch = AimRotator.Pitch;
		AimYaw = AimRotator.Yaw;
	}
}

void UTODAnimInstance::PlayAttackMontage()
{
	if (AttackMontage != nullptr)
		Montage_Play(AttackMontage, 1.0f);
}

void UTODAnimInstance::PlayHardAttackMontage()
{
	if (HardAttackMontage != nullptr)
		Montage_Play(HardAttackMontage, 1.0f);
}

void UTODAnimInstance::PlayThrowMontage()
{
	if (SpecialAttackThrowMontage != nullptr)
	{
		Montage_Play(SpecialAttackThrowMontage, 1.0f);

		IsEquip = false;
		IsSpecialAttacking = false;
	}
}

void UTODAnimInstance::PlayCatchMontage()
{
	if (SpecialAttackCatchMontage != nullptr)
	{
		Montage_Play(SpecialAttackCatchMontage, 1.0f);

		IsEquip = true;
		IsSpecialTarget = false;
	}
}

void UTODAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (Montage_IsPlaying(AttackMontage))
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UTODAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UTODAnimInstance::AnimNotify_SetCanAttack()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetCanAttack();
	}
}

void UTODAnimInstance::AnimNotify_AttackHitCheckStart()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetCanAttackDamage(true);
	}
}

void UTODAnimInstance::AnimNotify_AttackHitCheckEnd()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetCanAttackDamage(false);
	}
}

FName UTODAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, 4))
		return FName(*FString::Printf(TEXT("Attack%d"), Section));
	else
		return NAME_None;
}

void UTODAnimInstance::AnimNotify_HardAttackStart()
{
	Montage_SetPlayRate(HardAttackMontage, 0.3f);
}

void UTODAnimInstance::AnimNotify_HardAttackEnd()
{
	OnHardAttackEnd.Broadcast();
}

void UTODAnimInstance::AnimNotify_HardAttackHitCheck()
{
	OnHardAttackHitCheck.Broadcast(0, 400.0f);
}

void UTODAnimInstance::AnimNotify_SpecialTargeting()
{
	IsSpecialTarget = true;
}