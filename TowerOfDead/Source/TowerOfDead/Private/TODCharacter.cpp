#include "TODCharacter.h"
#include "TODCharacterStatComponent.h"
#include "TODAnimInstance.h"
#include "TODPlayerController.h"
#include "TODPlayerState.h"
#include "TODGameMode.h"
#include "TODUserWidget.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyStatComponent.h"
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
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bEnableCameraRotationLag = true;
	Camera->SetupAttachment(SpringArm);

	Decal->SetupAttachment(RootComponent);
	Decal->SetVisibility(false);
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	GetMesh()->bReceivesDecals = false;

	WeaponTrigger->SetupAttachment(GetMesh(), FName(TEXT("Weapon")));
	WeaponTrigger->SetGenerateOverlapEvents(false);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODCharacter"));

	// �⺻ ĳ���� �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/Kwang_GDC.Kwang_GDC"));
	if (SK_KWANG.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_KWANG.Object);

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM(TEXT("/Game/BluePrint/Anim_Player.Anim_Player_C"));
	if (KWANG_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);

	HitEffect->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/ParagonKwang/FX/Particles/Abilities/Primary/FX/P_Kwang_Primary_Impact.P_Kwang_Primary_Impact"));
	if (P_HITEFFECT.Succeeded())
	{
		HitEffect->SetTemplate(P_HITEFFECT.Object);
		HitEffect->bAutoActivate = false;
	}

	SetControl();
	
	IsDead = false;
	
	// �Ϲ� ���� ����
	IsAttacking = false;
	CanAttack = true;
	MaxCombo = 4;
	AttackEndComboState();

	// ���콺 ��Ŭ�� (������)
	IsHardAttacking = false;
	IsCanHardAttack = true;

	// R ��ư (Ư�� ����)
	IsSpecialAttacking = false;
	IsCanSpecialAttack = true;
	IsCanSpecialCatch = false;

	// ȸ�� or ���� Object
	IsSoulRecovery = false;
	IsCanStopSoulRecovery = false;
}

void ATODCharacter::BeginPlay()
{
	Super::BeginPlay();

	TODLOG_S(Warning);

	ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
	if (playerController != nullptr)
	{
		PlayerController = playerController;
		PlayerController->GetUserHUDWidget()->BindCharacterStatClass(CharacterStat);
	}

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

	// ������ ������
	if (CastTime <= HardAttackTime && IsHardAttacking)
	{
		CastTime += GetWorld()->DeltaTimeSeconds;
		OnHardAttackCast.Broadcast();

		if (CastTime > HardAttackTime)
			HardAttackCheck();
	}

	// Ư�� ���� ������ ��ǥ ��ġ ����
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
			// ���� ���� ǥ�� (��Į)
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			Decal->SetWorldLocation(Hit.Location);
			
			FVector Point = Hit.Location;
			Point.Z = 2000.0f;
			Anim->SetTargetPoint(Point);
		}
	}

	FVector EffectPoint = Anim->GetTargetPoint();
	EffectPoint.Z = 30.0f;
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

	TODLOG_S(Warning);

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
		
		Anim->OnHardAttackEnd.AddUObject(this, &ATODCharacter::HardAttackEnd);
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

	// ī�޶� ��ġ �� ���� : ī�޶��� �ε巯�� �������� ���ؼ�
	SpringArm->bEnableCameraLag = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->JumpZVelocity = 400.0f;
}

void ATODCharacter::SetPlayerStart(bool start)
{
	if (PlayerController == nullptr)
		return;

	PlayerController->SetCanInputAction(start);
}

void ATODCharacter::SetPlayerDead()
{
	// Ư�� ���� ���� ����(Decal)
	Decal->SetVisibility(false);
	IsSpecialAttacking = false;

	// ������ ���� ����
	IsHardAttacking = false;
	PlayerController->GetUserHUDWidget()->SetVisibleCast(false);

	// �̵� �Ұ�
	SetCharacterMove(false);
	IsDead = true;
	// Ű �Է� �Ұ�
	PlayerController->SetCanInputAction(false);
}

void ATODCharacter::SetCharacterMove(bool isMoveing)
{
	if (IsDead)
		isMoveing = false;

	PlayerController = Cast<ATODPlayerController>(GetController());
	if (PlayerController != nullptr)
		PlayerController->SetIsMove(isMoveing);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = isMoveing;
}

void ATODCharacter::SetBossEnemyStatBind(class UTODEnemyStatComponent* enemyStat)
{
	if (enemyStat == nullptr)
		return;

	PlayerController->GetUserHUDWidget()->BindBossEnemyStateClass(enemyStat);
}

void ATODCharacter::SoulRecovery()
{
	if (IsSoulRecovery)
		return;

	PlayerController->GetUserHUDWidget()->SetVisibleLevelUp(true);
	FInputModeGameAndUI GameAndUIInputMode;
	PlayerController->SetInputMode(GameAndUIInputMode);
	PlayerController->bShowMouseCursor = true;

	SetIsSoulRecovery(true);	

	// ���Ⱑ ������ �ִٸ� ĳġ ���� �� ȸ��
	if (Anim->GetIsSpecialTarget())
		SpecialAttackCatch();

	// �ִϸ��̼� Flag On
	Anim->SetIsSoulRecovery(true);
	// ü�� ȸ��
	CharacterStat->RecoveryHP();
}

void ATODCharacter::SoulRecoveryEnd()
{
	// SoulRecovery ���� �ƴҶ� ������ X
	if (!IsSoulRecovery || !IsCanStopSoulRecovery)
		return;

	PlayerController->GetUserHUDWidget()->SetVisibleLevelUp(false);
	FInputModeGameOnly GameInputMode;
	PlayerController->SetInputMode(GameInputMode);
	PlayerController->bShowMouseCursor = false;

	Anim->SetIsSoulRecovery(false);
}

void ATODCharacter::SetIsSoulRecovery(bool isSoulRecovery)
{
	// Soul Recovery ����
	if (isSoulRecovery)
	{
		// �̵� �Ұ�
		SetCharacterMove(false);
		// Ű �Է� �Ұ�
		PlayerController->SetCanInputAction(false);
	}
	// Soul Recovery ����
	else
	{
		// �̵� ����
		SetCharacterMove(true);
		// Ű �Է� ����
		PlayerController->SetCanInputAction(true);
	}

	IsSoulRecovery = isSoulRecovery;
	IsCanStopSoulRecovery = false;
}

void ATODCharacter::Attack()
{
	if (Anim == nullptr)
		return;

	// ���Ⱑ ������ �ִٸ� �Ұ��� (��� �������� �� �Ұ���)
	if (IsHardAttacking || IsSpecialAttacking || !CanAttack)
		return;

	if (IsAttacking)
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
		IsAttacking = true;
	}
}

void ATODCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// �Ϲ� ���� ��Ÿ�ֶ��
	if (Montage == Anim->GetAttackMontage())
	{
		IsAttacking = false;
		AttackEndComboState();
	}

	// �� ������ �� 
	if (Montage == Anim->GetHardAttackMontage())
	{
		CanAttack = true;
	}
}

void ATODCharacter::AttackStartComboState()
{
	ArrHitEnemyCheck.Empty();

	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo += 1;
}

void ATODCharacter::AttackEndComboState()
{
	ArrHitEnemyCheck.Empty();

	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

bool ATODCharacter::HitEnemyCheck(class ATODEnemy* enemy)
{
	bool isEnemy = ArrHitEnemyCheck.Contains(enemy);
	
	// ���� ó�� ���� ���Ͷ��
	if (!isEnemy)
	{
		ArrHitEnemyCheck.Add(enemy);
		return true;
	}
	else
		return false;
}

void ATODCharacter::HardAttack()
{
	// ������ ���� ���� Ȯ��
	if (IsCanHardAttack == false)
		return;

	// ���Ⱑ ������ �ִٸ� �Ұ���
	if (Anim->GetIsSpecialTarget() || IsSpecialAttacking)
		return;

	Anim->PlayHardAttackMontage();
	SetCharacterMove(false);

	CanAttack = false;
	IsHardAttacking = true;
	IsCanHardAttack = false;

	CastTime = 0.0f;

	PlayerController->GetUserHUDWidget()->SetVisibleCast(true);
}

void ATODCharacter::HardAttackCheck()
{
	// ������ �������� �ƴ϶�� ���� üũ�� �ʿ䰡 ����
	if (IsHardAttacking == false)
		return;

	IsHardAttacking = false;

	float percent = (CastTime / HardAttackTime);
	
	// ���� : ���� ��Ÿ�� ���
	if (percent >= 0.7f && percent <= 0.9f)
	{
		Anim->Montage_SetPlayRate(Anim->GetHardAttackMontage(), 1.0f);
		Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("HardAttack2"))),
			Anim->GetHardAttackMontage());
	}
	// ���� : ��Ÿ�� ���� �� �̵� �� ���� ����
	else
	{
		SetCharacterMove(true);
		Anim->Montage_Stop(0.2f, Anim->GetCurrentActiveMontage());
	}

	PlayerController->GetUserHUDWidget()->SetVisibleCast(false);

	GetWorldTimerManager().SetTimer(HardAttackTimerHandle, this,
		&ATODCharacter::HardAttackCoolDownTimer, HardAttackCoolDownTime, false);
}

void ATODCharacter::HardAttackEnd()
{
	SetCharacterMove(true);
	CanAttack = true;
}

void ATODCharacter::HardAttackCoolDownTimer()
{
	print(FString::Printf(TEXT("Can Hard Attack!!")));
	IsCanHardAttack = true;
	CastTime = 0.0f;
}

void ATODCharacter::SpecialAttack()
{
	// ���� ��
	if (IsAttacking || Anim->Montage_IsPlaying(Anim->GetHardAttackMontage()))
		return;

	// Ư�� ���� ����
	if (IsCanSpecialAttack)
	{
		Decal->SetVisibility(true);
		IsSpecialAttacking = true;
		IsCanSpecialAttack = false;
		IsCanSpecialCatch = false;
		CanAttack = false;

		// ���� �ð� ������
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
		if (PlayerController != nullptr)
			PlayerController->SetMouseSpeed(0.3f);

		GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = false;
		Anim->SetSpecialAttacking(true);
		//this->CustomTimeDilation = 1.0f;
	}
	// Ư�� ���� �Ұ��� (Į�� ������ �ִ� ����, ��Ÿ�� ���� ��)
	else
	{
		// Į�� ������ �ִ� ������ ��� & Catch�� ������ ���
		if (Anim->GetIsSpecialTarget() && IsCanSpecialCatch)
			SpecialAttackCatch();
	}
}

void ATODCharacter::SpecialAttackEnd()
{
	// Ư�� �������� �ƴ� ���
	if (IsSpecialAttacking == false)
		return;

	// �ð� �������
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
	if (PlayerController != nullptr)
		PlayerController->SetMouseSpeed(0.5f);

	Decal->SetVisibility(false);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	IsSpecialAttacking = false;
	IsWeaponFall = true;

	// ��ǥ ������ Į ��ô
	Anim->PlayThrowMontage();
}

void ATODCharacter::SpecialAttackCatch()
{
	// ���� �ޱ�
	Anim->PlayCatchMontage();
	
	// ��Ÿ�� ����
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

	// �ش� ���� ���� ������
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

	// Hard/Special Attack Channels (ECC_GameTraceChannel4)�� �ɸ��� ������Ʈ
	if (bResult)
	{
		for (auto OverlapActor : OverlapActors)
		{
			ATODEnemy* Enemy = Cast<ATODEnemy>(OverlapActor.GetActor());
			if (Enemy != nullptr)
			{
				bool isCanDamage = false;

				// Hard Attack : ���� 180���� ������ �ִ� �� ����
				if (AttackType == 0)
				{
					float Angle = FVector::DotProduct(GetActorForwardVector(), Enemy->GetActorLocation() - GetActorLocation());
					
					// ����(180��)�� �ִ� Enemy���Ը� ���� ó��
					if (Angle >= 0.0f)
						isCanDamage = true;
				}
				else if (AttackType == 1)
				{
					isCanDamage = true;
				}

				// ������ ������ Enemy
				if (isCanDamage)
				{
					FDamageEvent DamageEvent;
					Enemy->TakeDamage(15.0f, DamageEvent, GetController(), this);

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
		// ���ݹ��� ���� Ȯ��
		if (HitEnemyCheck(Enemy))
		{
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);

			FVector effectLoc = Enemy->GetMesh()->GetSocketLocation("Impact");
			HitEffect->SetWorldLocation(effectLoc);
			HitEffect->Activate(true);
		}
	}
}