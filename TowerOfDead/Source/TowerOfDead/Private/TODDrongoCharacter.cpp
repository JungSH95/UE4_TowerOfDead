#include "TODDrongoCharacter.h"
#include "TODDrongoAnimInstance.h"

ATODDrongoCharacter::ATODDrongoCharacter()
{
	// �⺻ ĳ���� �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_DRONGO(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Meshes/Drongo_GDC.Drongo_GDC"));
	if (SK_DRONGO.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_DRONGO.Object);

	IsCanAttack = true;

	IsBazookaAttacking = false;
	IsGrenadeAttacking = false;
}

void ATODDrongoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATODDrongoCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Anim = Cast<UTODDrongoAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr)
	{
		Anim->OnMontageEnded.AddDynamic(this, &ATODDrongoCharacter::OnAttackMontageEnded);
	}
}

float ATODDrongoCharacter::GetSkillCastRatio()
{
	return 0.0f;
}

void ATODDrongoCharacter::Attack()
{
	if (Anim == nullptr || !IsCanAttack || IsGrenadeAttacking || IsBazookaAttacking)
		return;

	SetIsBattle(true);
	IsCanAttack = false;
	Anim->PlayAttackMontage();

	GetWorldTimerManager().SetTimer(AttackDelayTimerHandle, this,
		&ATODDrongoCharacter::AttackDelayTime, 0.5f, false);

	GetWorldTimerManager().ClearTimer(IsBattleTimerHandle);
	GetWorldTimerManager().SetTimer(IsBattleTimerHandle, this, &ATODCharacter::BattleEnd, 5.0f, false);
}

void ATODDrongoCharacter::AttackDelayTime()
{
	IsCanAttack = true;
}

void ATODDrongoCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

}

// ����ź? ������
void ATODDrongoCharacter::ActionMouseRight()
{
	Cast<UTODDrongoAnimInstance>(Anim)->PlayGrenadePrepMontage();

	IsGrenadeAttacking = true;
}

void ATODDrongoCharacter::ActionMouseRightEnd()
{
	// ����ź�� ��� ���� �ʴٸ�
	if (!IsGrenadeAttacking)
		return;

	// ���콺 Ŀ�� �������� ����ź ������ (��Ÿ� ����) == Kwang�� Į ������
	Cast<UTODDrongoAnimInstance>(Anim)->SetIsGrenade(false);
}

// ����ī ��
void ATODDrongoCharacter::ActionKeyboardR()
{
	Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookzEquipMontage();

	IsBazookaAttacking = true;
}

void ATODDrongoCharacter::ActionKeyboardREnd()
{
	if (!IsBazookaAttacking)
		return;

	// ���콺 Ŀ�� �������� ����ī �߻� (��ź�� ����?)
	Cast<UTODDrongoAnimInstance>(Anim)->SetIsBazooka(false);
}
