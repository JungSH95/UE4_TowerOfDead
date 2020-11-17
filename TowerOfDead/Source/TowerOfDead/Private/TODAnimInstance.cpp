#include "TODAnimInstance.h"
#include "TODPlayerController.h"
#include "Kismet/KismetMathLibrary.h"

UTODAnimInstance::UTODAnimInstance()
{
	CurrentSpeed = 0.0f;
	CurrentPawnDirection = 0.0f;
	AimPitch = 0.0f;
	AimYaw = 0.0f;

	IsBattle = false;
	IsDead = false;
	IsAir = false;
	IsEquip = true;
	IsSoulRecovery = false;
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

		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		IsBattle = Player->GetIsBattle();
	}
}

void UTODAnimInstance::AnimNotify_SoulRecoveryLoopStart()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetIsCanStopSoulRecovery(true);
	}
}

void UTODAnimInstance::AnimNotify_SoulRecoveryEnd()
{
	auto PlayerPawn = TryGetPawnOwner();
	if (::IsValid(PlayerPawn))
	{
		ATODCharacter* Player = Cast<ATODCharacter>(PlayerPawn);
		if (Player != nullptr)
			Player->SetIsSoulRecovery(false);
	}
}
