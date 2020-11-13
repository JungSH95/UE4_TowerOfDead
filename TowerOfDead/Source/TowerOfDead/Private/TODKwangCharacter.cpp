#include "TODKwangCharacter.h"
#include "TODKwangAnimInstance.h"
#include "Enemy/TODEnemy.h"

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
}

void ATODKwangCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATODKwangCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

		//Anim->OnHardAttackEnd.AddUObject(this, &ATODCharacter::HardAttackEnd);
		//Anim->OnHardAttackHitCheck.AddUObject(this, &ATODCharacter::HardAndSpecialAttackHitCheck);
	}
}

void ATODKwangCharacter::Attack()
{
	print(FString::Printf(TEXT("Kwang Attack")));

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