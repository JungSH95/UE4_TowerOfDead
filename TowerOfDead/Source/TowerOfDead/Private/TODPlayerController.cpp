#include "TODPlayerController.h"
#include "TODPlayerState.h"
#include "TODEnemy.h"
#include "TODEnemyStatComponent.h"
#include "TODUserWidget.h"

ATODPlayerController::ATODPlayerController()
{
	static ConstructorHelpers::FClassFinder<UTODUserWidget> BP_UI(TEXT("/Game/UI/InGame_UI.InGame_UI_C"));
	if (BP_UI.Succeeded())
		HUDWidgetClass = BP_UI.Class;

	isMove = true;
	MouseSpeed = 0.5f;
	CanInputAction = false;
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
	InputComponent->BindAction(TEXT("SpecialAttack"), EInputEvent::IE_Released, this, &ATODPlayerController::SpecialAttackEnd);
}

void ATODPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TODLOG_S(Warning);

	CPlayer = Cast<ATODCharacter>(GetPawn());
	TODPlayerState = Cast<ATODPlayerState>(PlayerState);

	HUDWidget->BindPlayerClass(CPlayer);
	HUDWidget->BindPlayerStateClass(TODPlayerState);

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void ATODPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	TODLOG_S(Warning);
}

void ATODPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (HUDWidget == nullptr)
	{
		TODLOG_S(Warning);

		HUDWidget = CreateWidget<UTODUserWidget>(this, HUDWidgetClass);
		HUDWidget->AddToViewport(1);
	}
}

void ATODPlayerController::EnemyKill(class ATODEnemy* KilledEnemy)
{
	if (KilledEnemy == nullptr)
		return;

	TODPlayerState->AddSoul(KilledEnemy->EnemyStat->GetDropSoul());
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
	
	AddYawInput(AxisValue * MouseSpeed);
}

void ATODPlayerController::LookUp(float AxisValue)
{
	if (isMove == false)
		return;

	AddPitchInput(AxisValue * MouseSpeed);
}

void ATODPlayerController::Attack()
{
	if (CanInputAction == false)
		return;

	CPlayer->Attack();
}

void ATODPlayerController::HardAttack()
{
	if (CanInputAction == false)
		return;

	CPlayer->HardAttack();
}

void ATODPlayerController::HardAttackEnd()
{
	if (CanInputAction == false)
		return;

	// 버튼에서 땠을 때 판정을 확인하여 성공일 경우 강공격 실행 후 이동 및 점프 가능
	//									실패일 경우 Montage 정지 후 이동 및 점프 가능
	CPlayer->HardAttackCheck();
}

void ATODPlayerController::SpecialAttack()
{
	if (CanInputAction == false)
		return;

	CPlayer->SpecialAttack();
}

void ATODPlayerController::SpecialAttackEnd()
{
	if (CanInputAction == false)
		return;

	CPlayer->SpecialAttackEnd();
}