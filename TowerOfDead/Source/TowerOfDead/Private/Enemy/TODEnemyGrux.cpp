#include "Enemy/TODEnemyGrux.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "Enemy/TODEnemyGruxAIController.h"
#include "Enemy/TODGruxAIAnimInstance.h"
#include "TODCharacter.h"
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

	// 대쉬 피격 이펙트
	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DASHSKILLHITEFFECT(TEXT("/Game/ParagonGrux/FX/Particles/Skins/Grux_Beetle_Magma/P_Grux_Magma_Ultimate_Clang.P_Grux_Magma_Ultimate_Clang"));
	if (P_DASHSKILLHITEFFECT.Succeeded())
		DashSkillHitEffect = P_DASHSKILLHITEFFECT.Object;

	DashTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DashTriggerSphere"));
	DashTrigger->SetupAttachment(GetMesh());
	DashTrigger->SetGenerateOverlapEvents(false);
	
	HitEffect->bAutoActivate = false;

	IsDoubleAttacking = false;

	SkillDelayTime = 10.0f;

	IsCanMeteorSKill = false;
	MeteorSkillCoolDownTime = 6.0f;

	IsCanDashSKill = true;
	IsDashSKilling = false;
	DashSkillCoolDownTime = 10.0f;

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

void ATODEnemyGrux::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DashTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODEnemyGrux::OnDashTriggerOverlap);
}

void ATODEnemyGrux::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto AnimInstance = Cast<UTODGruxAIAnimInstance>(GetMesh()->GetAnimInstance());
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

		TODLOG_S(Warning);
	}
}

bool ATODEnemyGrux::GetIsCanOutRangeAttack()
{
	if (IsCanDashSKill == false)
		return false;

	if (!IsCanDashSKill || !IsCanMeteorSKill && !IsCanEnemySpawnSKill)
		return false;

	return true;
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

void ATODEnemyGrux::SkillDelayTimer()
{
	ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
	if (EnemyAI != nullptr)
		EnemyAI->SetIsCanOutRangeAttack(true);
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
	IsCanDashSKill = false;

	// 대시 유지 시간
	GetWorldTimerManager().SetTimer(DashSkillEndTimerHandle, this, &ATODEnemyGrux::DashSkillEndTimer,
		10.0f, false);

	// 쿨타임
	GetWorldTimerManager().SetTimer(DashSkillTimerHandle, this, &ATODEnemyGrux::DashSkillCoolDownTimer,
		DashSkillCoolDownTime, false);
}

void ATODEnemyGrux::OnDashSkillOverlap()
{
	DashTrigger->SetGenerateOverlapEvents(true);
}

void ATODEnemyGrux::DashSkillEndTimer()
{
	IsDashSKilling = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	DashTrigger->SetGenerateOverlapEvents(false);	
}

void ATODEnemyGrux::DashSkillCoolDownTimer()
{
	TODLOG_S(Warning);
	IsCanDashSKill = true;
}

void ATODEnemyGrux::StunEnd()
{
	ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
	if (EnemyAI != nullptr)
		EnemyAI->SetIsAttaking(false);
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

void ATODEnemyGrux::OutRangeAttack(float dis)
{
	// 어떤 기술을 사용할 것인지? & 해당 기술은 사용 가능한 상태인지

	// 1순위 - 거리가 멀고 & 대쉬 기술 사용 가능
	if (IsCanDashSKill)
	{
		DashSkill();

		// 다음 기술 사용까지의 딜레이
		GetWorldTimerManager().SetTimer(SkillDelayTimerHandle, this, &ATODEnemyGrux::SkillDelayTimer,
			SkillDelayTime, false);
		return;
	}

	// 일정 체력 이하 시 메테오, 몬스터 소환 기술 사용
	// -> 일단 테스트 하기위해 바로 사용

	return;
}

void ATODEnemyGrux::StartHitEffect(FVector pos)
{
	// Normal Attack Hit
	if (GetIsAttacking())
		HitEffect->SetTemplate(AttackHitEffect);

	// Double Attack 중 Hit
	if (IsDoubleAttacking)
		HitEffect->SetTemplate(DoubleAttackHitEffect);

	if(IsDashSKilling)
		HitEffect->SetTemplate(DashSkillHitEffect);

	HitEffect->SetWorldLocation(pos);
	HitEffect->Activate(true);
}

void ATODEnemyGrux::OnDashTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult)
{	
	ATODCharacter* Player = Cast<ATODCharacter>(OtherActor);
	if (Player != nullptr)
	{
		// 캐릭터 일 경우 처리
		if (Player->GetMovementComponent()->IsFalling())
			Player->GetCharacterMovement()->AddImpulse(this->GetActorForwardVector() * 2000.0f, true);
		else
			Player->GetCharacterMovement()->AddImpulse(this->GetActorForwardVector() * 20000.0f, true);

		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(EnemyStat->GetAttack(), DamageEvent, GetController(), this);

		FVector effectLoc = Player->GetMesh()->GetSocketLocation("Impact");
		StartHitEffect(effectLoc);

		DashSkillEndTimer();
		return;
	}

	// 나머지 환경 요소(벽 등) 충돌 시
	FVector effectLoc = GetMesh()->GetSocketLocation("FX_Head");
	StartHitEffect(effectLoc);

	DashSkillEndTimer();
}