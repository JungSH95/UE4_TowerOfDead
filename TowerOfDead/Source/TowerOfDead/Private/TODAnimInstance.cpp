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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/PlayerComboAttack_Montage.PlayerComboAttack_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
		AttackMontage = ATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HARDATTACK_MONTAGE(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/PlayerHardAttack_Montage.PlayerHardAttack_Montage"));
	if (HARDATTACK_MONTAGE.Succeeded())
		HardAttackMontage = HARDATTACK_MONTAGE.Object;	
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
	{
		Montage_Play(AttackMontage, 1.0f);
	}
}

void UTODAnimInstance::PlayHardAttackMontage()
{
	if (HardAttackMontage)
	{
		Montage_Play(HardAttackMontage, 1.0f);
	}
}

void UTODAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (Montage_IsPlaying(AttackMontage))
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UTODAnimInstance::AnimNotify_LevelStartAnimEnd()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		PlayerPawn->GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

		ATODPlayerController* player = Cast< ATODPlayerController>(PlayerPawn->GetController());
		if (player != nullptr)
			player->SetIsMove(true);
	}
}

void UTODAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
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
	OnHardAttackEnd.Broadcast(true);
}