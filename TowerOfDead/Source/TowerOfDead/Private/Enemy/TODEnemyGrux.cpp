#include "Enemy/TODEnemyGrux.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "Enemy/TODEnemyGruxAIController.h"
#include "Enemy/TODGruxAIAnimInstance.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"
#include "TODMeteor.h"
#include "NavigationSystem.h"
#include "Components/WidgetComponent.h"

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

	// 소환할 몬스터 1
	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_1(TEXT("/Game/BluePrint/Enemy/BP_Enemy_1.BP_Enemy_1_C"));
	if (ENEMYCLASS_1.Class != NULL)
		EnemyToSpawn = ENEMYCLASS_1.Class;

	// 소환할 Meteor
	ConstructorHelpers::FClassFinder<ATODMeteor> METEORCLASS(TEXT("/Game/BluePrint/BP_Meteor.BP_Meteor_C"));
	if (METEORCLASS.Class != NULL)
		MeteorToSpawn = METEORCLASS.Class;

	DashTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("DashTriggerSphere"));
	DashTrigger->SetupAttachment(GetMesh());
	DashTrigger->SetGenerateOverlapEvents(false);
	
	HitEffect->bAutoActivate = false;

	PhaseCount = 1;

	IsDoubleAttacking = false;

	SkillDelayTime = 5.0f;

	IsCanMeteorSKill = false;
	MeteorSkillCoolDownTime = 20.0f;

	IsCanDashSKill = false;
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

	EnemyStat->SetName("Magma Grux");
	EnemyStat->OnHPChanged.AddLambda([this]() -> void {
		// 몬스터 소환기술 사용 가능
		if (EnemyStat->GetHPRatio() <= 0.3f)
		{
			if (PhaseCount == 3)
			{
				PhaseCount++;
				IsCanEnemySpawnSKill = true;
			}
		}
		// 유성우 소환기술 사용 가능
		else if (EnemyStat->GetHPRatio() <= 0.6f)
		{
			if (PhaseCount == 2)
			{
				PhaseCount++;
				IsCanMeteorSKill = true;
			}
		}
		// 대쉬 기술 사용 가능
		else if (EnemyStat->GetHPRatio() <= 0.9f)
		{
			if (PhaseCount == 1)
			{
				PhaseCount++;
				IsCanDashSKill = true;
			}
		}
	});
}

float ATODEnemyGrux::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	print(FString::Printf(TEXT("Grux Enemy took Damage : %f"), FinalDamage));

	EnemyStat->SetDamage(FinalDamage);

	if (GetIsDead())
	{
		if (EventInstigator->IsPlayerController())
		{
			auto PlayerController = Cast<ATODPlayerController>(EventInstigator);
			if (PlayerController != nullptr)
				PlayerController->EnemyKill(this);
		}
	}

	return FinalDamage;
}

void ATODEnemyGrux::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto AnimInstance = Cast<UTODGruxAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;
	
	ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
	if (EnemyAI == nullptr)
		return;

	// 공격 몽타주가 끝났다면
	if (AnimInstance->IsAttackMontage(Montage))
	{
		EnemyAI->SetIsAttaking(false);
		AnimInstance->NowMontage = nullptr;

		AttackCoolDownTimerStart();

		print(FString::Printf(TEXT("Enemy Grux Attack Montage End")));
	}
	// 몬스터 소환 몽타주가 끝났다면
	else if (AnimInstance->IsEnemySpawnCastMontage(Montage))
	{
		EnemyAI->SetIsAttaking(false);

		print(FString::Printf(TEXT("Enemy Grux Enemy Spawn Montage End")));
	}
	else if (AnimInstance->IsMeteorCastMonatage(Montage))
		EnemyAI->SetIsAttaking(false);
}

bool ATODEnemyGrux::GetIsCanOutRangeAttack()
{
	// 스킬이 하나라도 사용이 가능하다면
	if (IsCanDashSKill || IsCanMeteorSKill || IsCanMeteorSKill)
		return true;

	return false;
}

void ATODEnemyGrux::LevelStartMontage()
{
	auto AnimInstance = Cast<UTODGruxAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	print(FString::Printf(TEXT("Enemy Grux Level Start Montage")));
	AnimInstance->PlayLevelStartMontage();
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

void ATODEnemyGrux::RandomPointInit(float distance, int count)
{
	RandomPoint.Empty();

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return;

	for (int i = 0; i < count; i++)
	{
		FNavLocation NewPoint;
		if (NavSystem->GetRandomPointInNavigableRadius(GetActorLocation(), distance, NewPoint))
			RandomPoint.Add(NewPoint.Location);
	}
}

void ATODEnemyGrux::MeteorSkill()
{
	IsCanMeteorSKill = false;
	RandomPointInit(3000.0f, 15);

	// 생성 안했다면 생성
	if (SpawnMeteors.Num() == 0)
	{
		for (int i = 0; i < 15; i++)
		{
			FVector NewLocation = RandomPoint[i];
			NewLocation.Z = 3000.0f;

			ATODMeteor* SpawnMeteor = GetWorld()->SpawnActor<ATODMeteor>(MeteorToSpawn, NewLocation,
				GetActorRotation());
			if (SpawnMeteor != nullptr)
			{
				SpawnMeteor->SetMeteor(3.0f);
				SpawnMeteors.Add(SpawnMeteor);
			}
		}
	}
	// 생성되있는거 재활용
	else
	{
		for (int i = 0; i < 15; i++)
		{
			FVector NewLocation = RandomPoint[i];
			NewLocation.Z = 3000.0f;

			SpawnMeteors[i]->SetActorLocation(NewLocation);
			SpawnMeteors[i]->SetMeteor(3.0f);
		}
	}

	print(FString::Printf(TEXT("Enemy Grux MeteorSkill Count : %d"), SpawnMeteors.Num()));

	GetWorldTimerManager().SetTimer(MeteorSkillTimerHandle, this, &ATODEnemyGrux::MeteorSkillCoolDownTimer,
		MeteorSkillCoolDownTime, false);
}

void ATODEnemyGrux::MeteorSkillCoolDownTimer()
{
	// 일정 시간마다 자동 사용
	MeteorSkill();
}

void ATODEnemyGrux::DashSkill()
{
	IsDashSKilling = true;
	IsCanDashSKill = false;

	// 대시 유지 시간
	GetWorldTimerManager().SetTimer(DashSkillEndTimerHandle, this, &ATODEnemyGrux::DashSkillEndTimer,
		5.0f, false);

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
	auto AnimInstance = Cast<UTODGruxAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	AnimInstance->PlayEnemySpawnCastMontage();
	IsCanEnemySpawnSKill = false;
	EnemySpawnCount = 0;

	SpawnEnemys.Empty();
	RandomPointInit(1500.0f, 5);

	for (int i = 0; i < 5; i++)
	{
		ATODEnemy* SpawnEnemy = GetWorld()->SpawnActor<ATODEnemy>(EnemyToSpawn, RandomPoint[i],
			GetActorRotation());
		if (SpawnEnemy != nullptr)
		{
			SpawnEnemy->EnemyStat->SetNewLevel(EnemyStat->GetLevel());
			SpawnEnemy->OnEnemyDeadCheck.AddUObject(this, &ATODEnemyGrux::EnemySpawnDeadCount);

			ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(SpawnEnemy->GetController());
			if (EnemyAI != nullptr)
				EnemyAI->StartAI();
			SpawnEnemy->HPBarWidget->SetHiddenInGame(false);

			SpawnEnemys.Add(SpawnEnemy);
		}
	}

	GetWorldTimerManager().SetTimer(EnemySpawnSkillTimerHandle, this, &ATODEnemyGrux::EnemySpawnSkillCoolDownTimer,
		EnemySpawnSkillCoolDownTime, false);
}

void ATODEnemyGrux::EnemySpawnDeadCount()
{
	EnemySpawnCount++;

	if (EnemySpawnCount >= SpawnEnemys.Num())
		EnemySpawnCount = SpawnEnemys.Num();
}

void ATODEnemyGrux::EnemySpawnSkillCoolDownTimer()
{
	if (EnemyStat->GetHPRatio() <= 0.6f)
	{
		print(FString::Printf(TEXT("Enemy Grux : Can Enemy Spawn Skill")));
		IsCanEnemySpawnSKill = true;
	}
}

void ATODEnemyGrux::OutRangeAttack(float dis)
{
	// 다음 기술 사용까지의 딜레이
	ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
	if (EnemyAI != nullptr)
		EnemyAI->SetIsCanOutRangeAttack(false);
	
	GetWorldTimerManager().SetTimer(SkillDelayTimerHandle, this, &ATODEnemyGrux::SkillDelayTimer,
		SkillDelayTime, false);

	// 어떤 기술을 사용할 것인지? & 해당 기술은 사용 가능한 상태인지

	// 필수 패턴 (몬스터 소환 - 스킬 사용 가능하면서 소환된 몬스터가 없다면)
	if (IsCanEnemySpawnSKill && (SpawnEnemys.Num() == EnemySpawnCount))
	{
		EnemySpawnSkill();
		return;
	}

	// 체력 50퍼 남았을 때 시전 후 지속
	if (IsCanMeteorSKill)
	{
		auto AnimInstance = Cast<UTODGruxAIAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance == nullptr)
			return;
		AnimInstance->PlayMeteorCastMontage();

		MeteorSkill();
		return;
	}

	// 거리가 멀고 & 대쉬 기술 사용 가능
	if (IsCanDashSKill)
	{
		DashSkill();
		return;
	}

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
	print(OtherActor->GetName());

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

	// 나머지 환경 요소(벽) 충돌 시
	if (OtherActor->ActorHasTag("Wall"))
	{
		FVector effectLoc = GetMesh()->GetSocketLocation("FX_Head");
		StartHitEffect(effectLoc);
		DashSkillEndTimer();
	}
}