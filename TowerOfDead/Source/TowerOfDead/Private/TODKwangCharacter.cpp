#include "TODKwangCharacter.h"
#include "TODKwangAnimInstance.h"
#include "TODPlayerController.h"
#include "TODUserWidget.h"
#include "Enemy/TODEnemy.h"
#include "Components/DecalComponent.h"

ATODKwangCharacter::ATODKwangCharacter()
{
	// 기본 캐릭터 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood"));
	if (SK_KWANG.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_KWANG.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM(TEXT("/Game/BluePrint/Anim_KwangPlayer.Anim_KwangPlayer_C"));
	if (KWANG_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/ParagonKwang/FX/Particles/Abilities/Primary/FX/P_Kwang_Primary_Impact.P_Kwang_Primary_Impact"));
	if (P_HITEFFECT.Succeeded())
		HitEffect->SetTemplate(P_HITEFFECT.Object);

	// 일반 공격 설정
	IsAttacking = false;
	//CanAttack = true;
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

void ATODKwangCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATODKwangCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 강공격 진행중
	if (CastTime <= HardAttackTime && IsHardAttacking)
	{
		CastTime += GetWorld()->DeltaTimeSeconds;
		OnHardAttackCast.Broadcast();

		// 실패
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
	// 사용중인 기술을 확인하여 해당 기술의 캐스트 비율을 반환 
	if (IsHardAttacking)
		return CastTime / HardAttackTime;

	return 0.0f;
}

void ATODKwangCharacter::Attack()
{
	if (Anim == nullptr)
		return;

	// 무기가 던져져 있다면 불가능 (기술 시전중일 때 불가능)
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

	// 공격 처리 안한 몬스터라면
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
	// 일반 공격 몽타주라면
	if (Montage == Anim->GetAttackMontage())
	{
		IsAttacking = false;
		AttackEndComboState();
	}

	// 강 공격일 때 
	//if (Montage == Anim->GetHardAttackMontage())
	//{
	//	CanAttack = true;
	//}
}

void ATODKwangCharacter::ActionMouseRight()
{
	// 강공격 가능 여부 확인
	if (IsCanHardAttack == false)
		return;

	// 무기가 던져져 있다면 불가능
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
	// 공격 중
	if (IsAttacking || Anim->Montage_IsPlaying(Anim->GetHardAttackMontage()))
		return;

	// 특수 공격 가능
	if (IsCanSpecialAttack)
	{
		Decal->SetVisibility(true);
		IsSpecialAttacking = true;
		IsCanSpecialAttack = false;
		IsCanSpecialCatch = false;
		//CanAttack = false;

		// 세계 시간 느리게
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
		if (PlayerController != nullptr)
			PlayerController->SetMouseSpeed(0.3f);

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

void ATODKwangCharacter::SpecialAttackEnd()
{
	// 특수 공격중이 아닌 경우
	if (IsSpecialAttacking == false)
		return;

	// 시간 원래대로
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
	if (PlayerController != nullptr)
		PlayerController->SetMouseSpeed(0.5f);

	Decal->SetVisibility(false);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	IsSpecialAttacking = false;
	IsWeaponFall = true;

	// 목표 지점에 칼 투척
	Anim->PlayThrowMontage();
}

void ATODKwangCharacter::SpecialAttackCatch()
{
	// 무기 받기
	Anim->PlayCatchMontage();

	// 쿨타임 진행
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