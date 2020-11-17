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

	GetWorldTimerManager().ClearTimer(IsBattleTimerHandle);
	GetWorldTimerManager().SetTimer(IsBattleTimerHandle, this, &ATODCharacter::BattleEnd, 5.0f, false);
}

void ATODDrongoCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{

}

void ATODDrongoCharacter::ActionMouseRight()
{

}

void ATODDrongoCharacter::ActionMouseRightEnd()
{

}

void ATODDrongoCharacter::ActionKeyboardR()
{

}

void ATODDrongoCharacter::ActionKeyboardREnd()
{

}
