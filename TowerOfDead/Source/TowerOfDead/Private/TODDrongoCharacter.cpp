#include "TODDrongoCharacter.h"
#include "TODDrongoAnimInstance.h"

ATODDrongoCharacter::ATODDrongoCharacter()
{
	// 기본 캐릭터 메시
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

	// 바주카 공격
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

// 수류탄? 던지기
void ATODDrongoCharacter::ActionMouseRight()
{
	Cast<UTODDrongoAnimInstance>(Anim)->PlayGrenadePrepMontage();

	IsGrenadeAttacking = true;
}

void ATODDrongoCharacter::ActionMouseRightEnd()
{
	// 수류탄을 들고 있지 않다면
	if (!IsGrenadeAttacking)
		return;

	// 마우스 커서 지점으로 수류탄 던지기 (사거리 제한) == Kwang의 칼 던지기
	Cast<UTODDrongoAnimInstance>(Anim)->SetIsGrenade(false);
	IsGrenadeAttacking = false;
}

// 바주카 포
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
	// 마우스 커서 방향으로 바주카 발사 (산탄총 느낌?)
	Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookFireMontage();

	IsBazookaAttacking = false;

	// 넉백 and 띄우기
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