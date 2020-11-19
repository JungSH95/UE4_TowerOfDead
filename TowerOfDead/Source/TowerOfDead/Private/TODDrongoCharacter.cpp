#include "TODDrongoCharacter.h"
#include "TODDrongoAnimInstance.h"

ATODDrongoCharacter::ATODDrongoCharacter()
{
	// 기본 캐릭터 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_DRONGO(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Meshes/Drongo_GDC.Drongo_GDC"));
	if (SK_DRONGO.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_DRONGO.Object);

	IsCanAttack = true;
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
	if (Anim == nullptr || !IsCanAttack)
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
}

void ATODDrongoCharacter::ActionMouseRightEnd()
{
	Cast<UTODDrongoAnimInstance>(Anim)->SetIsGrenade(false);
}

// 바주카 포
void ATODDrongoCharacter::ActionKeyboardR()
{
	Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookzEquipMontage();
}

void ATODDrongoCharacter::ActionKeyboardREnd()
{
	Cast<UTODDrongoAnimInstance>(Anim)->SetIsBazooka(false);
}
