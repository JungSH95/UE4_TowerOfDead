#include "TODKwangCharacter.h"
#include "TODKwangAnimInstance.h"
#include "TODPlayerController.h"
#include "TODUserWidget.h"
#include "Enemy/TODEnemy.h"
#include "Components/DecalComponent.h"

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
			HardAttackEnd();
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
	//if (IsHardAttacking || IsSpecialAttacking || !CanAttack)
	//	return;

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

	// �� ������ �� 
	//if (Montage == Anim->GetHardAttackMontage())
	//{
	//	CanAttack = true;
	//}
}

void ATODKwangCharacter::ActionMouseRight()
{
	// ������ ���� ���� Ȯ��
	if (IsCanHardAttack == false)
		return;

	// ���Ⱑ ������ �ִٸ� �Ұ���
	//if (Anim->GetIsSpecialTarget() || IsSpecialAttacking)
	//	return;

	Anim->PlayHardAttackMontage();
	SetCharacterMove(false);

	//CanAttack = false;
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
		&ATODKwangCharacter::HardAttackCoolDownTimer, HardAttackCoolDownTime, false);
}

void ATODKwangCharacter::ActionKeyboardR()
{
	print(FString::Printf(TEXT("Kwang ActionKeyboardR")));
}

void ATODKwangCharacter::ActionKeyboardREnd()
{
	print(FString::Printf(TEXT("Kwang ActionKeyboardR End")));
}

void ATODKwangCharacter::HardAttackEnd()
{
	SetCharacterMove(true);
	//CanAttack = true;
}

void ATODKwangCharacter::HardAttackCoolDownTimer()
{
	print(FString::Printf(TEXT("Can Hard Attack!!")));
	IsCanHardAttack = true;
	CastTime = 0.0f;
}

/*
void ATODKwangCharacter::SpecialAttack()
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
		//CanAttack = false;

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

void ATODKwangCharacter::SpecialAttackEnd()
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

void ATODKwangCharacter::SpecialAttackCatch()
{
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
*/