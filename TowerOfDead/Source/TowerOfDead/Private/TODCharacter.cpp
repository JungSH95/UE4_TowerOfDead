#include "TODCharacter.h"
#include "TODCharacterStatComponent.h"
#include "TODAnimInstance.h"
#include "TODPlayerController.h"
#include "TODPlayerState.h"
#include "TODGameMode.h"
#include "TODUserWidget.h"
#include "TODEnemy.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/SkeletalMeshSocket.h"

ATODCharacter::ATODCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterStat = CreateDefaultSubobject<UTODCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	SpringArm = CreateDefaultSubobject <USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject <UCameraComponent>(TEXT("CAMERA"));
	Decal = CreateDefaultSubobject <UDecalComponent>(TEXT("DECAL"));
	WeaponTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WEAPONTRIGGER"));
	SwordEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bEnableCameraRotationLag = true;
	Camera->SetupAttachment(SpringArm);

	Decal->SetupAttachment(RootComponent);
	Decal->SetVisibility(false);
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	GetMesh()->bReceivesDecals = false;

	WeaponTrigger->SetupAttachment(GetMesh(), FName(TEXT("Weapon")));
	WeaponTrigger->SetGenerateOverlapEvents(false);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODCharacter"));

	// 기본 캐릭터 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC"));
	if (SK_KWANG.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_KWANG.Object);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM(TEXT("/Game/BluePrint/Anim_Player.Anim_Player_C"));
	if (KWANG_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);

	SetControl();
	
	IsDead = false;
	
	// 일반 공격 설정
	IsAttaking = false;
	MaxCombo = 4;
	AttackEndComboState();

	// 마우스 우클릭 (강공격)
	IsHardAttacking = false;
	IsCanHardAttack = true;

	// R 버튼 (특수 공격)
	IsSpecialAttacking = false;
	IsCanSpecialAttack = true;
	IsCanSpecialCatch = false;
}

void ATODCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		gameMode->GetUserHUDWidget()->BindPlayerClass(this);

	auto TODPlayerState = Cast<ATODPlayerState>(GetPlayerState());
	if (TODPlayerState != nullptr)
	{
		CharacterStat->SetNewCharacterStat(
			TODPlayerState->GetHPLevel(),
			TODPlayerState->GetATKLevel(),
			TODPlayerState->GetDEFLevel(),
			GetWorld()
		);
	}
}

void ATODCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead)
	{
		if (SpringArm->TargetArmLength <= 1000.0f)
			SpringArm->TargetArmLength += (250.0f * GetWorld()->DeltaTimeSeconds);

		return;
	}

	// 강공격 진행중
	if (CastTime <= HardAttackTime && IsHardAttacking)
	{
		CastTime += GetWorld()->DeltaTimeSeconds;
		OnHardAttackCast.Broadcast();

		if (CastTime > HardAttackTime)
			HardAttackCheck();
	}

	// 특수 공격 진행중 목표 위치 설정
	if (IsSpecialAttacking)
	{
		FVector StartPos = Camera->GetComponentLocation();
		FVector EndPos = Camera->GetForwardVector() * 2000.0f + StartPos;

		FHitResult Hit;
		FCollisionQueryParams TraceParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos,
			ECollisionChannel::ECC_GameTraceChannel3, TraceParams);

		if (bHit)
		{
			// 공격 범위 표시 (데칼)
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			Decal->SetWorldLocation(Hit.Location);
			
			FVector Point = Hit.Location;
			Point.Z = 2000.0f;
			Anim->SetTargetPoint(Point);
			IsWeaponFall = true;
		}
	}

	FVector EffectPoint = Anim->GetTargetPoint();
	EffectPoint.Z = 20.0f;
	SwordEffect->SetWorldLocation(EffectPoint);

	if (IsWeaponFall)
	{
		FalldeltaTime += GetWorld()->GetDeltaSeconds();

		FVector StartPos = Anim->GetTargetPoint();
		StartPos.Z = StartPos.Z - (FalldeltaTime * 15.0f);
		FVector TargetPos = Anim->GetTargetPoint();
		TargetPos.Z = 100.0f;

		if (StartPos.Z <= 100.0f)
		{
			SwordEffect->Activate(true);

			FalldeltaTime = 0.0f;
			IsWeaponFall = false;

			HardAndSpecialAttackHitCheck(1, 200.0f);

			GetWorldTimerManager().SetTimer(SpecialCatchTimerHandle, this,
				&ATODCharacter::SpecialAttackCatchTimer, 1.0f, false);
		}

		Anim->SetTargetPoint(StartPos);
	}
}

void ATODCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		Anim->SetIsDead();
		SetActorEnableCollision(false);

		SetPlayerDead();
	});

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
		Anim->OnHardAttackHitCheck.AddUObject(this, &ATODCharacter::HardAndSpecialAttackHitCheck);
	}

	WeaponTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODCharacter::OnWewaponTriggerOverlap);
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
	CharacterStat->SetDamage(FinalDamage);
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
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->JumpZVelocity = 400.0f;
}

void ATODCharacter::SetPlayerDead()
{
	TODLOG_S(Warning);

	// 특수 공격 관련 종료(Decal)
	Decal->SetVisibility(false);
	IsSpecialAttacking = false;

	// 강공격 관련 종료
	IsHardAttacking = false;
	
	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		gameMode->GetUserHUDWidget()->SetVisibleCast(false);

	// 이동 불가
	SetCharacterMove(false);
	IsDead = true;
}

void ATODCharacter::SetCharacterMove(bool isMoveing)
{
	if (IsDead)
		isMoveing = false;

	ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
	if (playerController != nullptr)
		playerController->SetIsMove(isMoveing);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = isMoveing;
}

void ATODCharacter::Attack()
{
	if (Anim == nullptr)
		return;

	// 무기가 던져져 있다면 불가능
	if (Anim->GetIsSpecialTarget() || IsHardAttacking || IsSpecialAttacking)
		return;

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

	// 무기가 던져져 있다면 불가능
	if (Anim->GetIsSpecialTarget() || IsSpecialAttacking)
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

	GetWorldTimerManager().SetTimer(HardAttackTimerHandle, this,
		&ATODCharacter::HardAttackCoolDownTimer, HardAttackCoolDownTime, false);
}

void ATODCharacter::HardAttackCoolDownTimer()
{
	print(FString::Printf(TEXT("Can Hard Attack!!")));
	IsCanHardAttack = true;
	CastTime = 0.0f;
}

void ATODCharacter::SpecialAttack()
{
	// 공격 중
	if (IsAttaking || Anim->Montage_IsPlaying(Anim->GetHardAttackMontage()))
		return;

	// 특수 공격 가능
	if (IsCanSpecialAttack)
	{
		Decal->SetVisibility(true);
		IsSpecialAttacking = true;
		IsCanSpecialAttack = false;
		IsCanSpecialCatch = false;

		// 세계 시간 느리게
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
		ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
		if (playerController != nullptr)
			playerController->SetMouseSpeed(0.3f);

		GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = false;
		Anim->SetSpecialAttacking(true);
		//this->CustomTimeDilation = 1.0f;
	}
	// 특수 공격 불가능 (칼을 던지고 있는 상태, 쿨타임 진행 중)
	else
	{
		// 칼을 던지고 있는 상태일 경우 & Catch가 가능할 경우
		if (Anim->GetIsSpecialTarget() && IsCanSpecialCatch)
			SpecialAttackCatch();
	}
}

void ATODCharacter::SpecialAttackEnd()
{
	// 특수 공격중이 아닌 경우
	if (IsSpecialAttacking == false)
		return;

	// 시간 원래대로
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
	ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
	if (playerController != nullptr)
		playerController->SetMouseSpeed(0.5f);

	Decal->SetVisibility(false);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	IsSpecialAttacking = false;

	// 목표 지점에 칼 투척
	Anim->PlayThrowMontage();
}

void ATODCharacter::SpecialAttackCatch()
{
	// 무기 받기
	Anim->PlayCatchMontage();

	// 쿨타임 진행
	GetWorldTimerManager().SetTimer(SpecialAttackTimerHandle, this,
		&ATODCharacter::SpecialAttackCoolDownTimer, SpecialAttackCoolDownTime, false);
}

void ATODCharacter::SpecialAttackCatchTimer()
{
	IsCanSpecialCatch = true;
}

void ATODCharacter::SpecialAttackCoolDownTimer()
{
	IsCanSpecialAttack = true;
}

void ATODCharacter::HardAndSpecialAttackHitCheck(int32 AttackType, float Range)
{
	// Attack Type Check
	FVector VTarget;
	switch (AttackType)
	{
	// Hard Attack
	case 0:
		VTarget = GetActorLocation();
		break;
	// Special Attack
	case 1:
		VTarget = Anim->GetTargetPoint();
		break;
	default:
		VTarget = GetActorLocation();
		break;
	}

	// 해당 범위 몬스터 얻어오기
	TArray<FOverlapResult> OverlapActors;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapActors,
		VTarget,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(Range),
		CollisionQueryParam
	);

	DrawDebugSphere(GetWorld(), VTarget, Range, 16, FColor::Red, false, 10.0f);

	// Hard/Special Attack Channels (ECC_GameTraceChannel4)에 걸리는 오브젝트
	if (bResult)
	{
		for (auto OverlapActor : OverlapActors)
		{
			ATODEnemy* Enemy = Cast<ATODEnemy>(OverlapActor.GetActor());
			if (Enemy != nullptr)
			{
				bool isCanDamage = false;

				// Hard Attack : 전방 180도의 범위에 있는 적 공격
				if (AttackType == 0)
				{
					float Angle = FVector::DotProduct(GetActorForwardVector(), Enemy->GetActorLocation() - GetActorLocation());
					
					// 정면(180도)에 있는 Enemy에게만 공격 처리
					if (Angle >= 0.0f)
						isCanDamage = true;
				}
				else if (AttackType == 1)
				{
					isCanDamage = true;
				}

				// 공격이 가능한 Enemy
				if (isCanDamage)
				{
					FDamageEvent DamageEvent;
					Enemy->TakeDamage(50.0f, DamageEvent, GetController(), this);

					DrawDebugLine(GetWorld(), VTarget, Enemy->GetActorLocation(),
						FColor::Blue, false, 5.0f);
				}
			}
		}
	}
}

void ATODCharacter::OnWewaponTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult)
{
	ATODEnemy* Enemy = Cast<ATODEnemy>(OtherActor);
	if (Enemy != nullptr)
	{
		FDamageEvent DamageEvent;
		Enemy->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
	}
}