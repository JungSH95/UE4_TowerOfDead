#include "Enemy/TODEnemyGrux.h"
#include "Enemy/TODEnemyGruxAIController.h"
#include "Enemy/TODAIAnimInstance.h"
#include "NavigationSystem.h"

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

	IsCanMeteorSKill = false;
	MeteorSkillCoolDownTime = 60.0f;
	
	//GetWorldTimerManager().SetTimer(MeteorSkillTimerHandle, this, &ATODEnemyGrux::MeteorSkillCoolDownTimer,
	//	MeteorSkillCoolDownTime, false);

	IsCanDashSKill = true;
	IsDashSKilling = false;
	DashSkillCoolDownTime = 30.0f;

	IsCanEnemySpawnSKill = false;
	EnemySpawnSkillCoolDownTime = 90.0f;
	
	//GetWorldTimerManager().SetTimer(EnemySpawnSkillTimerHandle, this, &ATODEnemyGrux::EnemySpawnSkillCoolDownTimer,
	//	EnemySpawnSkillCoolDownTime, false);
}

void ATODEnemyGrux::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDashSKilling)
	{
		GetCharacterMovement()->MaxWalkSpeed = 2000.0f;
		AddMovementInput(GetActorForwardVector(), 1.0f);
	}
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

void ATODEnemyGrux::RandomPointInit(int count)
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	FNavLocation NewPoint;
	if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), 1500.0f, NewPoint))
	{
		if (ActorToSpawn == nullptr)
			return;
		
		RandomPoint.Add(NewPoint.Location);

		FActorSpawnParameters SpawnParams;
		AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(ActorToSpawn, NewPoint.Location,
			FRotator::ZeroRotator, SpawnParams);
	}
}

void ATODEnemyGrux::MeteorSkillCoolDownTimer()
{
	IsCanMeteorSKill = true;
}

void ATODEnemyGrux::DashSkill()
{
	IsDashSKilling = true;
	IsCanMeteorSKill = false;

	GetWorldTimerManager().SetTimer(DashSkillTimerHandle, this, &ATODEnemyGrux::DashSkillCoolDownTimer,
		DashSkillCoolDownTime, false);
}

void ATODEnemyGrux::DashSkillCoolDownTimer()
{
	IsCanDashSKill = true;
}

void ATODEnemyGrux::EnemySpawnSkillCoolDownTimer()
{
	IsCanEnemySpawnSKill = true;
}

bool ATODEnemyGrux::OutRangeAttack(float dis)
{
	//TODLOG_S(Warning);

	// 어떤 기술을 사용할 것인지? & 해당 기술은 사용 가능한 상태인지

	// 1순위 - 거리가 멀고 & 대쉬 기술 사용 가능
	if (dis >= 500.0f && IsCanDashSKill)
	{
		DashSkill();
		return true;
	}

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