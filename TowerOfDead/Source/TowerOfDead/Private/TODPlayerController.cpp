#include "TODPlayerController.h"
#include "TODPlayerState.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "TODUserWidget.h"
#include "TODGameMode.h"

ATODPlayerController::ATODPlayerController()
{
	static ConstructorHelpers::FClassFinder<UTODUserWidget> BP_UI(TEXT("/Game/UI/InGame_UI.InGame_UI_C"));
	if (BP_UI.Succeeded())
		HUDWidgetClass = BP_UI.Class;

	IsMove = true;
	MouseSpeed = 0.5f;
	CanInputAction = false;
	IsCanObjectInteraction = false;
}


void ATODPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("UpDown"), this, &ATODPlayerController::UpDown);
	InputComponent->BindAxis(TEXT("LeftRight"), this, &ATODPlayerController::LeftRight);
	InputComponent->BindAxis(TEXT("Turn"), this, &ATODPlayerController::Turn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &ATODPlayerController::LookUp);

	InputComponent->BindAction(TEXT("Mouse_Left"), EInputEvent::IE_Pressed, this, &ATODPlayerController::InputActionMouseLeft);

	InputComponent->BindAction(TEXT("Mouse_Right"), EInputEvent::IE_Pressed, this, &ATODPlayerController::InputActionMouseRight);
	InputComponent->BindAction(TEXT("Mouse_Right"), EInputEvent::IE_Released, this, &ATODPlayerController::InputActionMouseRightEnd);
	
	InputComponent->BindAction(TEXT("KeyBoard_R"), EInputEvent::IE_Pressed, this, &ATODPlayerController::InputActionKeyboardR);
	InputComponent->BindAction(TEXT("KeyBoard_R"), EInputEvent::IE_Released, this, &ATODPlayerController::InputActionKeyboardREnd);

	InputComponent->BindAction(TEXT("KeyBoard_F"), EInputEvent::IE_Pressed, this, &ATODPlayerController::ObjInteraction);
}

void ATODPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TODLOG_S(Warning);

	CPlayer = Cast<ATODCharacter>(GetPawn());
	TODPlayerState = Cast<ATODPlayerState>(PlayerState);

	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		HUDWidget->BindGameModeClass(gameMode);
	if (CPlayer != nullptr)
		HUDWidget->BindPlayerClass(CPlayer);
	if (TODPlayerState != nullptr)
		HUDWidget->BindPlayerStateClass(TODPlayerState);
	TODPlayerState->OnPlayerStateChange.Broadcast();

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
	TODPlayerState->OnPlayerStateChange.Broadcast();
}

void ATODPlayerController::UpDown(float AxisValue)
{	
	if (CPlayer != nullptr)
		if (CPlayer->GetIsCanStopSoulRecovery() && AxisValue != 0.0f)
			CPlayer->SoulRecoveryEndAction();

	if (IsMove == false)
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
	if (CPlayer != nullptr)
		if (CPlayer->GetIsCanStopSoulRecovery() && AxisValue != 0.0f)
			CPlayer->SoulRecoveryEndAction();

	if (IsMove == false)
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
	if (IsMove == false)
		return;
	
	AddYawInput(AxisValue * MouseSpeed);
}

void ATODPlayerController::LookUp(float AxisValue)
{
	if (IsMove == false)
		return;
	
	AddPitchInput(AxisValue * MouseSpeed);
}

void ATODPlayerController::InputActionMouseLeft()
{
	if (CanInputAction == false)
		return;

	CPlayer->Attack();
}

void ATODPlayerController::InputActionMouseRight()
{
	if (CanInputAction == false)
		return;

	CPlayer->ActionMouseRight();
}

void ATODPlayerController::InputActionMouseRightEnd()
{
	if (CanInputAction == false)
		return;

	// 버튼에서 땠을 때 판정을 확인하여 성공일 경우 강공격 실행 후 이동 및 점프 가능
	//									실패일 경우 Montage 정지 후 이동 및 점프 가능
	CPlayer->ActionMouseRightEnd();
}

void ATODPlayerController::InputActionKeyboardR()
{
	if (CanInputAction == false)
		return;

	CPlayer->ActionKeyboardR();
}

void ATODPlayerController::InputActionKeyboardREnd()
{
	if (CanInputAction == false)
		return;

	CPlayer->ActionKeyboardREnd();
}

void ATODPlayerController::ObjInteraction()
{
	if (IsCanObjectInteraction == false)
		return;
	else
		// 회복 진행
		CPlayer->SoulRecovery();
}