#include "Enemy/TODEnemyMelee.h"
#include "Enemy/TODEnemyMeleeAIController.h"
#include "Enemy/TODAIAnimInstance.h"

ATODEnemyMelee::ATODEnemyMelee()
{
	AIControllerClass = ATODEnemyMeleeAIController::StaticClass();
	// 레벨에 배치하거나 새롭게 생성되는 Enemy마다 ANNSEnemyAIController액터 생성 및 지배를 받는다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/ParagonMinions/FX/Particles/Minions/Minion_melee/FX/Impacts/P_Minion_Impact_Default.P_Minion_Impact_Default"));
	if (P_HITEFFECT.Succeeded())
	{
		HitEffect->SetTemplate(P_HITEFFECT.Object);
		HitEffect->bAutoActivate = false;
	}
}

void ATODEnemyMelee::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	// NowMontage에는 공격 몽타주가 저장되어 있다.
	if (AnimInstance->IsAttackMontage(Montage))
	{
		ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
		if (EnemyAI == nullptr)
			return;

		EnemyAI->SetIsAttaking(false);
		AnimInstance->NowMontage = nullptr;

		AttackCoolDownTimerStart();
	}
}

void ATODEnemyMelee::Attack()
{
	if (!GetIsCanAttack())
		return;

	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	AnimInstance->PlayAttackMontage();
	SetIsCanAttack(false);
}

void ATODEnemyMelee::StartHitEffect(FVector pos)
{
	HitEffect->SetWorldLocation(pos);
	HitEffect->Activate(true);
}