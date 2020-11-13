#include "TODKwangCharacter.h"
#include "TODKwangAnimInstance.h"
#include "Enemy/TODEnemy.h"

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