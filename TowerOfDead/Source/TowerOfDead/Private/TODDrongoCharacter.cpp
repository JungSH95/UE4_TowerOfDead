#include "TODDrongoCharacter.h"
#include "TODDrongoAnimInstance.h"

ATODDrongoCharacter::ATODDrongoCharacter()
{
	// �⺻ ĳ���� �޽�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_DRONGO(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Meshes/Drongo_GDC.Drongo_GDC"));
	if (SK_DRONGO.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_DRONGO.Object);

	IsCanAttack = true;

	IsCanBazookaAttack = true;
	IsBazookaAttacking = false;

	IsGrenadeAttacking = false;
}

void ATODDrongoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGrenadeAttacking)
	{

	}
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
	if (Anim == nullptr || !IsCanAttack || IsGrenadeAttacking)
		return;

	// ����ī ����
	if (IsBazookaAttacking)
	{
		IsCanAttack = false;

		BazookaFire();
		return;
	}

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
	IsGrenadeAttacking = false;
}

// ����ī ��
void ATODDrongoCharacter::ActionKeyboardR()
{
	if (IsCanBazookaAttack)
	{
		Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookzEquipMontage();

		IsCanBazookaAttack = false;
		IsBazookaAttacking = true;
	}
}

void ATODDrongoCharacter::ActionKeyboardREnd()
{
	return;
}

void ATODDrongoCharacter::BazookaFire()
{
	// ���콺 Ŀ�� �������� ����ī �߻� (��ź�� ����?)
	Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookFireMontage();

	IsBazookaAttacking = false;

	// �˹� and ����
	FVector AddForce = GetActorForwardVector() * -500.0f + FVector(0, 0, 1) * 500.0f;
	LaunchCharacter(AddForce, true, false);

	GetWorldTimerManager().SetTimer(BazookaDelayTimerHandle, this,
		&ATODDrongoCharacter::BazookaDelayTimer, 5.0f, false);
}

void ATODDrongoCharacter::BazookaDelayTimer()
{
	print(FString::Printf(TEXT("Is Can Bazooka Skill")));
	IsCanBazookaAttack = true;
}