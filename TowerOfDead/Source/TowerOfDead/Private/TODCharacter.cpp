#include "TODCharacter.h"
#include "TODAnimInstance.h"
#include "TODPlayerController.h"

#include "TODGameMode.h"
#include "TODUserWidget.h"

#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"

#include "Engine/SkeletalMeshSocket.h"

ATODCharacter::ATODCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject <USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject <UCameraComponent>(TEXT("CAMERA"));
	Decal = CreateDefaultSubobject <UDecalComponent>(TEXT("DECAL"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bEnableCameraRotationLag = true;
	Camera->SetupAttachment(SpringArm);

	Decal->SetupAttachment(RootComponent);
	Decal->SetVisibility(false);
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SetControl();

	// 기본 캐릭터 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC"));
	if (SK_KWANG.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_KWANG.Object);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM(TEXT("/Game/BluePrint/Anim_Player.Anim_Player_C"));
	if (KWANG_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);

	static ConstructorHelpers::FObjectFinder<UMaterial> INVISIBLEWEAPON_MATERIAL(TEXT("/Game/InvisibleMaterial.InvisibleMaterial"));
	if (INVISIBLEWEAPON_MATERIAL.Succeeded())
		InVisibleWeaponMaterial = INVISIBLEWEAPON_MATERIAL.Object;

	static ConstructorHelpers::FObjectFinder<UMaterial> VISIBLEWEAPON_MATERIAL(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Materials/M_Kwang_Weapon.M_Kwang_Weapon"));
	if (VISIBLEWEAPON_MATERIAL.Succeeded())
		VisibleWeaponMaterial = VISIBLEWEAPON_MATERIAL.Object;
	
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->JumpZVelocity = 400.0f;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODCharacter"));

	IsAttaking = false;
	MaxCombo = 4;
	AttackEndComboState();

	IsHardAttacking = false;
	IsCanHardAttack = true;

	IsSpecialttacking = false;
}

void ATODCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		gameMode->GetUserHUDWidget()->BindPlayerClass(this);

	//print(FString::Printf(TEXT("Materials : %d"), GetMesh()->GetNumMaterials()));
	//GetMesh()->SetMaterial(17, InVisibleWeaponMaterial);	
}

void ATODCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 강공격 진행중
	if (CastTime <= HardAttackTime && IsHardAttacking)
	{
		CastTime += GetWorld()->DeltaTimeSeconds;
		OnHardAttackCast.Broadcast();

		if (CastTime > HardAttackTime)
			HardAttackCheck();
	}

	// 특수 공격 진행중 목표 위치 설정
	if (IsSpecialttacking)
	{
		FVector StartPos = Camera->GetComponentLocation();
		FVector EndPos = Camera->GetForwardVector() * 2000.0f + StartPos;

		FHitResult Hit;
		FCollisionQueryParams TraceParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos,
			ECollisionChannel::ECC_GameTraceChannel3, TraceParams);

		//DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Orange, false, 2.0f);

		if (bHit)
		{
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			Decal->SetWorldLocation(Hit.Location);
			// 공격 범위 표시 (데칼)
		}
	}
}

void ATODCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Anim = Cast<UTODAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr)
	{
		Anim->OnMontageEnded.AddDynamic(this, &ATODCharacter::OnAttackMontageEnded);
		Anim->OnNextAttackCheck.AddLambda([this]()->void 
		{
			CanNextCombo = true;

			if (IsComboInputOn)
			{
				AttackStartComboState();
				Anim->JumpToAttackMontageSection(CurrentCombo);
			}
		});
		
		Anim->OnHardAttackEnd.AddUFunction(this, FName("SetCharacterMove"));
	}
}

void ATODCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATODCharacter::Jump);
}

float ATODCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	return FinalDamage;
}

void ATODCharacter::SetControl()
{
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 70.0f);

	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;

	// 카메라 위치 렉 설정 : 카메라의 부드러운 움직임을 위해서
	SpringArm->bEnableCameraLag = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
}

void ATODCharacter::Attack()
{
	if (Anim == nullptr)
		return;

	if (GetMovementComponent()->IsFalling())
	{
		// 공중 공격 따로 처리
		return;
	}

	if (IsAttaking)
	{
		if (CanNextCombo)
		{
			AttackStartComboState();

			if (CurrentCombo > MaxCombo)
				return;

			Anim->JumpToAttackMontageSection(CurrentCombo);
		}
	}
	else
	{
		AttackStartComboState();
		Anim->PlayAttackMontage();
		Anim->JumpToAttackMontageSection(CurrentCombo);
		IsAttaking = true;
	}
}

void ATODCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 일반 공격 몽타주라면
	if (Montage == Anim->GetAttackMontage())
	{
		IsAttaking = false;
		AttackEndComboState();
	}
}

void ATODCharacter::AttackStartComboState()
{
	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo += 1;
}

void ATODCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ATODCharacter::HardAttack()
{
	// 강공격 가능 여부 확인
	if (IsCanHardAttack == false)
		return;

	Anim->PlayHardAttackMontage();
	SetCharacterMove(false);

	IsHardAttacking = true;
	IsCanHardAttack = false;

	CastTime = 0.0f;

	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		gameMode->GetUserHUDWidget()->SetVisibleCast(true);
}

void ATODCharacter::HardAttackCheck()
{
	// 강공격 진행중이 아니라면 판정 체크할 필요가 없음
	if (IsHardAttacking == false)
		return;

	IsHardAttacking = false;

	float percent = (CastTime / HardAttackTime);
	
	// 성공 : 다음 몽타주 재생
	if (percent >= 0.7f && percent <= 0.9f)
	{
		Anim->Montage_SetPlayRate(Anim->GetHardAttackMontage(), 1.0f);
		Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("HardAttack2"))),
			Anim->GetHardAttackMontage());
	}
	// 실패 : 몽타주 정지 후 이동 및 점프 가능
	else
	{
		SetCharacterMove(true);
		Anim->Montage_Stop(0.2f, Anim->GetCurrentActiveMontage());
	}

	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		gameMode->GetUserHUDWidget()->SetVisibleCast(false);

	GetWorldTimerManager().SetTimer(HardAttackTimerHandle, this, &ATODCharacter::HardAttackCoolDownTimer, HardAttackCoolDownTime, false);
}

void ATODCharacter::SetCharacterMove(bool isMoveing)
{
	ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
	if (playerController != nullptr)
		playerController->SetIsMove(isMoveing);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = isMoveing;
}

void ATODCharacter::HardAttackCoolDownTimer()
{
	print(FString::Printf(TEXT("Can Hard Attack!!")));
	IsCanHardAttack = true;
	CastTime = 0.0f;
}

void ATODCharacter::SpecialAttack()
{
	if (IsSpecialttacking)
		return;

	Decal->SetVisibility(true);
	IsSpecialttacking = true;

	// 세계 시간 느리게
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
	ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
	if (playerController != nullptr)
		playerController->SetMouseSpeed(0.3f);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = false;
	Anim->SetSpecialAttacking(true);
	//this->CustomTimeDilation = 1.0f;
}

void ATODCharacter::SpecialAttackEnd()
{
	// 시간 원래대로
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
	ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
	if (playerController != nullptr)
		playerController->SetMouseSpeed(0.5f);

	Decal->SetVisibility(false);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	// 목표 지점에 칼 투척
	Anim->PlayThrowMontage();

	// 일정 시간 뒤 IsSpecialttacking를 false로 (쿨타임)
	IsSpecialttacking = false;
}