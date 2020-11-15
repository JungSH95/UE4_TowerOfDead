#include "TODKwangCharacter.h"
#include "TODKwangAnimInstance.h"
#include "TODPlayerController.h"
#include "TODUserWidget.h"
#include "Enemy/TODEnemy.h"
#include "Components/DecalComponent.h"
#include "DrawDebugHelpers.h"

ATODKwangCharacter::ATODKwangCharacter()
{
	// �⺻ ĳ���� �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood"));
	if (SK_KWANG.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_KWANG.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM(TEXT("/Game/BluePrint/Anim_KwangPlayer.Anim_KwangPlayer_C"));
	if (KWANG_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/ParagonKwang/FX/Particles/Abilities/Primary/FX/P_Kwang_Primary_Impact.P_Kwang_Primary_Impact"));
	if (P_HITEFFECT.Succeeded())
		HitEffect->SetTemplate(P_HITEFFECT.Object);

	// �Ϲ� ���� ����
	IsAttacking = false;
	//CanAttack = true;
	MaxCombo = 4;
	AttackEndComboState();

	// ���콺 ��Ŭ�� (������)
	IsHardAttacking = false;
	IsCanHardAttack = true;

	// R ��ư (Ư�� ����)
	IsSpecialAttacking = false;
	IsCanSpecialAttack = true;
	IsCanSpecialCatch = false;
}

void ATODKwangCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATODKwangCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ������
	if (CastTime <= HardAttackTime && IsHardAttacking)
	{
		CastTime += GetWorld()->DeltaTimeSeconds;
		OnHardAttackCast.Broadcast();

		// ����
		if (CastTime > HardAttackTime)
		{
			if (Anim != nullptr)
				Anim->Montage_Stop(0.2f, Anim->GetCurrentActiveMontage());
			HardAttackEnd();
		}
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
			Point.Z = 1500.0f;
			if (Anim != nullptr)
				Anim->SetTargetPoint(Point);
		}
	}

	// Į ���� ����
	if (IsWeaponFall && Anim != nullptr)
	{
		FalldeltaTime += GetWorld()->GetDeltaSeconds();

		// Į ���� ��ġ
		FVector StartPos = Anim->GetTargetPoint();
		StartPos.Z = StartPos.Z - (FalldeltaTime * 25.0f);
		FVector TargetPos = Anim->GetTargetPoint();
		TargetPos.Z = 100.0f;

		if (StartPos.Z >= 1100.0f && StartPos.Z <= 1200.0f)
			SwordEffect->Activate(true);

		if (StartPos.Z <= 100.0f)
		{
			FalldeltaTime = 0.0f;
			IsWeaponFall = false;

			HardAndSpecialAttackHitCheck(1, 200.0f);

			GetWorldTimerManager().SetTimer(SpecialCatchTimerHandle, this,
				&ATODKwangCharacter::SpecialAttackCatchTimer, 1.0f, false);
		}

		Anim->SetTargetPoint(StartPos);
	}
}

void ATODKwangCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
	Anim = Cast<UTODKwangAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr)
	{
		Anim->OnMontageEnded.AddDynamic(this, &ATODKwangCharacter::OnAttackMontageEnded);
		Anim->OnNextAttackCheck.AddLambda([this]()->void
		{
			CanNextCombo = true;

			if (IsComboInputOn)
			{
				AttackStartComboState();
				Anim->JumpToAttackMontageSection(CurrentCombo);
			}
		});

		Anim->OnHardAttackEnd.AddUObject(this, &ATODKwangCharacter::HardAttackEnd);
		//Anim->OnHardAttackHitCheck.AddUObject(this, &ATODKwangCharacter::HardAndSpecialAttackHitCheck);
	}
}

float ATODKwangCharacter::GetCastSkillRatio()
{
	// ������� ����� Ȯ���Ͽ� �ش� ����� ĳ��Ʈ ������ ��ȯ 
	if (IsHardAttacking)
		return CastTime / HardAttackTime;

	return 0.0f;
}

void ATODKwangCharacter::Attack()
{
	if (Anim == nullptr)
		return;

	// ���Ⱑ ������ �ִٸ� �Ұ��� (��� �������� �� �Ұ���)
	if (IsHardAttacking || IsSpecialAttacking || !Anim->GetIsEquip())
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

void ATODKwangCharacter::AttackStartComboState()
{
	ArrHitEnemyCheck.Empty();

	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo += 1;
}

void ATODKwangCharacter::AttackEndComboState()
{
	ArrHitEnemyCheck.Empty();

	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

bool ATODKwangCharacter::HitEnemyCheck(class ATODEnemy* enemy)
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

void ATODKwangCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// �Ϲ� ���� ��Ÿ�ֶ��
	if (Montage == Anim->GetAttackMontage())
	{
		IsAttacking = false;
		AttackEndComboState();
	}
}

void ATODKwangCharacter::ActionMouseRight()
{
	// ������ ���� ���� Ȯ��
	if (IsCanHardAttack == false)
		return;

	// ���Ⱑ X & �Ұ��� ��� ��� �� �Ұ���
	if (!Anim->GetIsEquip() || IsSpecialAttacking)
		return;

	Anim->PlayHardAttackMontage();
	SetCharacterMove(false);

	IsHardAttacking = true;
	IsCanHardAttack = false;

	CastTime = 0.0f;

	PlayerController->GetUserHUDWidget()->SetVisibleCast(true);
}

void ATODKwangCharacter::ActionMouseRightEnd()
{
	// ������ �������� �ƴ϶�� ���� üũ�� �ʿ䰡 ����
	if (IsHardAttacking == false)
		return;

	float percent = (CastTime / HardAttackTime);

	// ���� : ���� ��Ÿ�� ���
	if (percent >= 0.7f && percent <= 0.9f)
	{
		CastTime += 2.0f;	// Tick���� HardAttack �ȵ��ư���

		Anim->Montage_SetPlayRate(Anim->GetHardAttackMontage(), 1.0f);
		Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("HardAttack2"))),
			Anim->GetHardAttackMontage());

		if (PlayerController != nullptr)
			PlayerController->GetUserHUDWidget()->SetVisibleCast(false);
	}
	// ���� : ��Ÿ�� ���� �� �̵� �� ���� ����
	else
	{
		Anim->Montage_Stop(0.2f, Anim->GetCurrentActiveMontage());
		HardAttackEnd();
	}
}

void ATODKwangCharacter::HardAttackEnd()
{
	print(FString::Printf(TEXT("Kwang HardAttack End")));

	IsHardAttacking = false;
	SetCharacterMove(true);

	if (PlayerController != nullptr)
		PlayerController->GetUserHUDWidget()->SetVisibleCast(false);

	GetWorldTimerManager().SetTimer(HardAttackTimerHandle, this,
		&ATODKwangCharacter::HardAttackCoolDownTimer, HardAttackCoolDownTime, false);
}

void ATODKwangCharacter::HardAttackCoolDownTimer()
{
	print(FString::Printf(TEXT("Can Hard Attack!!")));
	IsCanHardAttack = true;
	CastTime = 0.0f;
}

void ATODKwangCharacter::ActionKeyboardR()
{
	// ���� �� �Ұ�
	if (IsAttacking || Anim->Montage_IsPlaying(Anim->GetHardAttackMontage()))
		return;
	
	// Ư�� ���� ����
	if (IsCanSpecialAttack)
	{
		Decal->SetVisibility(true);

		// ��ġ ���� ����(Tick)
		IsSpecialAttacking = true;
		// ��� ��� ���� ���� (��Ÿ��)
		IsCanSpecialAttack = false;
		// ĳġ ������ �������� 
		IsCanSpecialCatch = false;

		// ���� �ð� ������
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
		if (PlayerController != nullptr)
			PlayerController->SetMouseSpeed(0.3f);

		GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = false;
		Anim->SetSpecialAttacking(true);
	}
	// Ư�� ���� �Ұ��� (Į�� ������ �ִ� ����, ��Ÿ�� ���� ��)
	else
	{
		// Į�� ������ �ִ� ������ ��� & Catch�� ������ ���
		if (Anim->GetIsSpecialTarget() && IsCanSpecialCatch)
			SpecialAttackCatch();
	}
}

void ATODKwangCharacter::ActionKeyboardREnd()
{
	print(FString::Printf(TEXT("Kwang ActionKeyboardR End")));

	// Ư�� �������� �ƴ� ���
	if (IsSpecialAttacking == false)
		return;

	// �ð� �������
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
	if (PlayerController != nullptr)
		PlayerController->SetMouseSpeed(0.5f);

	Decal->SetVisibility(false);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	// ��ġ ���� ��(Tick)
	IsSpecialAttacking = false;
	// Į ���� ����
	IsWeaponFall = true;

	// Į ���� ����Ʈ ��ġ ����
	FVector EffectPoint = Anim->GetTargetPoint();
	EffectPoint.Z = 30.0f;
	SwordEffect->SetWorldLocation(EffectPoint);
	//SwordEffect->Activate(true);

	// ��ǥ ������ Į ��ô
	Anim->PlayThrowMontage();
}

void ATODKwangCharacter::SpecialAttackCatch()
{
	IsCanSpecialCatch = false;

	// ���� �ޱ�
	Anim->PlayCatchMontage();

	// ��Ÿ�� ����
	GetWorldTimerManager().SetTimer(SpecialAttackTimerHandle, this,
		&ATODKwangCharacter::SpecialAttackCoolDownTimer, SpecialAttackCoolDownTime, false);
}

void ATODKwangCharacter::SpecialAttackCatchTimer()
{
	IsCanSpecialCatch = true;
}

void ATODKwangCharacter::SpecialAttackCoolDownTimer()
{
	IsCanSpecialAttack = true;
}
