#include "Enemy/TODEnemyGrux.h"
#include "Enemy/TODEnemyGruxAIController.h"
#include "Enemy/TODAIAnimInstance.h"

ATODEnemyGrux::ATODEnemyGrux()
{
	AIControllerClass = ATODEnemyGruxAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 일반 공격 이펙트
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_ATTACKHITEFFECT(TEXT("/Game/ParagonGrux/FX/Particles/Abilities/Primary/FX/P_Grux_ApplyBleed.P_Grux_ApplyBleed"));
	if (P_ATTACKHITEFFECT.Succeeded())
		AttackHitEffect = P_ATTACKHITEFFECT.Object;

	// 강 공격 이펙트
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DOUBLEATTACKHITEFFECT(TEXT("/Game/ParagonGrux/FX/Particles/Abilities/Primary/FX/P_Grux_Melee_ShockwaveImpact.P_Grux_Melee_ShockwaveImpact"));
	if (P_DOUBLEATTACKHITEFFECT.Succeeded())
		DoubleAttackHitEffect = P_DOUBLEATTACKHITEFFECT.Object;

	HitEffect->bAutoActivate = false;

	IsDoubleAttacking = false;
	IsCanDashAttack = true;
}

void ATODEnemyGrux::Attack()
{
	if (!GetIsCanAttack())
		return;

	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	AnimInstance->PlayAttackMontage();
	SetIsCanAttack(false);
}

void ATODEnemyGrux::DoubleAttackHitCheck()
{
	IsDoubleAttacking = true;

	if(GetAttackTrigger() != nullptr)
		GetAttackTrigger()->SetGenerateOverlapEvents(true);
}

void ATODEnemyGrux::DoubleAttackHitCheckEnd()
{
	IsDoubleAttacking = false;

	if (GetAttackTrigger() != nullptr)
		GetAttackTrigger()->SetGenerateOverlapEvents(false);
}

bool ATODEnemyGrux::OutRangeAttack()
{
	// 어떤 기술을 사용할 것인지? & 해당 기술은 사용 가능한 상태인지

	// 1순위 - 거리가 멀다면 대쉬 기술

	return false;
}

void ATODEnemyGrux::StartHitEffect(FVector pos)
{
	if (GetIsAttacking())
		HitEffect->SetTemplate(AttackHitEffect);

	// Double Attack 중 Hit
	if (IsDoubleAttacking)
		HitEffect->SetTemplate(DoubleAttackHitEffect);

	HitEffect->SetWorldLocation(pos);
	HitEffect->Activate(true);
}