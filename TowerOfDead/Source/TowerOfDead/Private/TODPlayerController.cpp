#include "TODPlayerController.h"
#include "DrawDebugHelpers.h"

ATODPlayerController::ATODPlayerController()
{
	isMove = true;
}


void ATODPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("UpDown"), this, &ATODPlayerController::UpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &ATODPlayerController::LeftRight);
	InputComponent->BindAxis(TEXT("Turn"), this, &ATODPlayerController::Turn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &ATODPlayerController::LookUp);

	InputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ATODPlayerController::Attack);

	InputComponent->BindAction(TEXT("HardAttack"), EInputEvent::IE_Pressed, this, &ATODPlayerController::HardAttack);
	InputComponent->BindAction(TEXT("HardAttack"), EInputEvent::IE_Released, this, &ATODPlayerController::HardAttackEnd);
	InputComponent->BindAction(TEXT("SpecialAttack"), EInputEvent::IE_Pressed, this, &ATODPlayerController::SpecialAttack);
}

void ATODPlayerController::UpDown(float AxisValue)
{	
	if (isMove == false)
		return;

	if (CPlayer == nullptr)
		CPlayer = Cast<ATODCharacter>(GetPawn());
	else if(AxisValue != 0.0f)
	{
		const FRotator Rotation = ControlRotation;
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		CPlayer->AddMovementInput(Direction, AxisValue);
	}
}

void ATODPlayerController::LeftRight(float AxisValue)
{
	if (isMove == false)
		return;

	if (CPlayer == nullptr)
		CPlayer = Cast<ATODCharacter>(GetPawn());
	else if (AxisValue != 0.0f)
	{
		const FRotator Rotation = ControlRotation;
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		CPlayer->AddMovementInput(Direction, AxisValue);
	}
}

void ATODPlayerController::Turn(float AxisValue)
{
	if (isMove == false)
		return;

	AddYawInput(AxisValue);
}

void ATODPlayerController::LookUp(float AxisValue)
{
	if (isMove == false)
		return;

	AddPitchInput(AxisValue);
}

void ATODPlayerController::Attack()
{
	CPlayer->Attack();
}

void ATODPlayerController::HardAttack()
{
	if (!CPlayer->GetIsHardAttacking())
	{
		isMove = false;
		CPlayer->GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = false;
		CPlayer->HardAttack();
	}
}

void ATODPlayerController::HardAttackEnd()
{
	// 버튼에서 땠을 때 판정을 확인하여 성공일 경우 강공격 실행 후 이동 및 점프 가능
	//									실패일 경우 Montage 정지 후 이동 및 점프 가능
	CPlayer->HardAttackCheck();
}

void ATODPlayerController::SpecialAttack()
{
	TODLOG_S(Warning);

	FVector StartPos = CPlayer->Camera->GetComponentLocation();
	FVector EndPos = (CPlayer->Camera->GetForwardVector() * 10000.0f) + StartPos;

	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos,
		ECollisionChannel::ECC_GameTraceChannel3, TraceParams);

	DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Orange, false, 2.0f);
	
	if (bHit)
	{
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);

		// 공격 범위 표시 (데칼)
	}
}