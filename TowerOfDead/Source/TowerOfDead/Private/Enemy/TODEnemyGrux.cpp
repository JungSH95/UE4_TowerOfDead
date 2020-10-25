#include "Enemy/TODEnemyGrux.h"
#include "Enemy/TODEnemyGruxAIController.h"
#include "Enemy/TODGruxAIAnimInstance.h"
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

	DashTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DashTriggerSphere"));
	DashTrigger->SetupAttachment(GetMesh());
	DashTrigger->SetGenerateOverlapEvents(false);
	
	HitEffect->bAutoActivate = false;

	IsDoubleAttacking = false;

	IsCanMeteorSKill = false;
	MeteorSkillCoolDownTime = 6.0f;

	IsCanDashSKill = true;
	IsDashSKilling = false;
	DashSkillCoolDownTime = 3.0f;

	IsCanEnemySpawnSKill = false;
	EnemySpawnSkillCoolDownTime = 9.0f;
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

void ATODEnemyGrux::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto AnimInstance = Cast<UTODGruxAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	// NowMontage에는 공격 몽타주가 저장되어 있다.
	if (AnimInstance->NowMontage == Montage)
	{
		ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
		if (EnemyAI == nullptr)
			return;

		EnemyAI->SetIsAttaking(false);
		AnimInstance->NowMontage = nullptr;

		AttackCoolDownTimerStart();
	}
}

void ATODEnemyGrux::StartAllSkillCoolDown()
{
	// StartAI와 동시에 스킬 쿨타임 진행
	GetWorldTimerManager().SetTimer(DashSkillTimerHandle, this, &ATODEnemyGrux::DashSkillCoolDownTimer,
		DashSkillCoolDownTime, false);

	GetWorldTimerManager().SetTimer(MeteorSkillTimerHandle, this, &ATODEnemyGrux::MeteorSkillCoolDownTimer,
		MeteorSkillCoolDownTime, false);

	GetWorldTimerManager().SetTimer(EnemySpawnSkillTimerHandle, this, &ATODEnemyGrux::EnemySpawnSkillCoolDownTimer,
		EnemySpawnSkillCoolDownTime, false);
}

void ATODEnemyGrux::Attack()
{
	if (!GetIsCanAttack())
		return;

	auto AnimInstance = Cast<UTODGruxAIAnimInstance>(GetMesh()->GetAnimInstance());
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

		//FActorSpawnParameters SpawnParams;
		//AActor* SpawnedActorRef = GetWorld()->SpawnActor<AActor>(ActorToSpawn, NewPoint.Location,
		//	FRotator::ZeroRotator, SpawnParams);
	}
}

void ATODEnemyGrux::MeteorSkill()
{
	IsCanMeteorSKill = false;

	GetWorldTimerManager().SetTimer(MeteorSkillTimerHandle, this, &ATODEnemyGrux::MeteorSkillCoolDownTimer,
		MeteorSkillCoolDownTime, false);
}

void ATODEnemyGrux::MeteorSkillCoolDownTimer()
{
	TODLOG_S(Warning);
	IsCanMeteorSKill = true;
}

void ATODEnemyGrux::DashSkill()
{
	IsDashSKilling = true;
	IsCanMeteorSKill = false;

	// 대시 유지 시간
	//GetWorldTimerManager().SetTimer(DashSkillEndTimerHandle, this, &ATODEnemyGrux::DashSkillEndTimer,
	//	3.0f, false);

	// 쿨타임
	GetWorldTimerManager().SetTimer(DashSkillTimerHandle, this, &ATODEnemyGrux::DashSkillCoolDownTimer,
		DashSkillCoolDownTime, false);
}

void ATODEnemyGrux::DashSkillEndTimer()
{
	IsDashSKilling = false;
}

void ATODEnemyGrux::DashSkillCoolDownTimer()
{
	TODLOG_S(Warning);
	IsCanDashSKill = true;
}

void ATODEnemyGrux::EnemySpawnSkill()
{
	IsCanEnemySpawnSKill = false;

	GetWorldTimerManager().SetTimer(EnemySpawnSkillTimerHandle, this, &ATODEnemyGrux::EnemySpawnSkillCoolDownTimer,
		EnemySpawnSkillCoolDownTime, false);
}

void ATODEnemyGrux::EnemySpawnSkillCoolDownTimer()
{
	TODLOG_S(Warning);
	IsCanEnemySpawnSKill = true;
}

bool ATODEnemyGrux::OutRangeAttack(float dis)
{
	// 모든 기술 사용 불가능 상태
	if (!IsCanDashSKill && !IsCanMeteorSKill && !IsCanEnemySpawnSKill)
		return false;

	// 어떤 기술을 사용할 것인지? & 해당 기술은 사용 가능한 상태인지

	// 1순위 - 거리가 멀고 & 대쉬 기술 사용 가능
	if (dis >= 500.0f && IsCanDashSKill)
	{
		DashSkill();
		return true;
	}

	// 일정 체력 이하 시 메테오, 몬스터 소환 기술 사용
	// -> 일단 테스트 하기위해 바로 사용

	return false;
}

void ATODEnemyGrux::StartHitEffect(FVector pos)
{
	// Normal Attack Hit
	if (GetIsAttacking())
		HitEffect->SetTemplate(AttackHitEffect);

	// Double Attack 중 Hit
	if (IsDoubleAttacking)
		HitEffect->SetTemplate(DoubleAttackHitEffect);

	HitEffect->SetWorldLocation(pos);
	HitEffect->Activate(true);
}