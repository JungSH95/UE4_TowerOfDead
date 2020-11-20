#include "TODDrongoCharacter.h"
#include "TODDrongoAnimInstance.h"

ATODDrongoCharacter::ATODDrongoCharacter()
{
	// 기본 캐릭터 메시
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
}

// 바주카 포
void ATODDrongoCharacter::ActionKeyboardR()
{
	Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookzEquipMontage();

	IsBazookaAttacking = true;
}

void ATODDrongoCharacter::ActionKeyboardREnd()
{
	if (!IsBazookaAttacking)
		return;

	// 마우스 커서 방향으로 바주카 발사 (산탄총 느낌?)
	Cast<UTODDrongoAnimInstance>(Anim)->SetIsBazooka(false);
}
