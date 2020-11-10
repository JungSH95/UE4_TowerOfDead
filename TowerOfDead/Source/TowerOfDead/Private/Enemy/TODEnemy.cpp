#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "Enemy/TODEnemyAIController.h"
#include "Enemy/TODAIAnimInstance.h"
#include "TODCharacter.h"
#include "Components/WidgetComponent.h"
#include "Enemy/TODEnemyWidget.h"

ATODEnemy::ATODEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODEnemy"));
	
	GetMesh()->bReceivesDecals = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	
	AttackTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackTriggerBox"));
	AttackTrigger->SetupAttachment(GetMesh());
	AttackTrigger->SetGenerateOverlapEvents(false);
	
	EnemyStat = CreateDefaultSubobject<UTODEnemyStatComponent>(TEXT("ENEMYSTAT"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh(), FName(TEXT("HealthBar")));
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/EnemyHPBar_UI.EnemyHPBar_UI_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}
	// 보이는 유무 설정
	HPBarWidget->SetHiddenInGame(true);

	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));
	HitEffect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_DEADEFFECT(TEXT("/Game/ParagonMinions/FX/Particles/SharedGameplay/States/Death/FX/P_Death_Default.P_Death_Default"));
	if (P_DEADEFFECT.Succeeded())
		DeadEffect = P_DEADEFFECT.Object;

	State = EnemyState::PEACE;
	IsDead = false;

	AttackRange = 300.0f;
	EffectiveRange = AttackRange + 100.0f;

	IsCanAttack = true;
	NormalAttackCoolDownTime = 3.0f;
}

void ATODEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// HPBar Enemy 연동
	auto EnemyWidget = Cast<UTODEnemyWidget>(HPBarWidget->GetUserWidgetObject());
	if (EnemyWidget != nullptr)
		EnemyWidget->BindEnemyStat(EnemyStat);
}

void ATODEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATODEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	AnimInstance->OnMontageEnded.AddDynamic(this, &ATODEnemy::OnAttackMontageEnded);
	
	AttackTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODEnemy::OnAttackTriggerOverlap);

	EnemyStat->OnHPIsZero.AddLambda([this]() -> void {
		ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
		if (EnemyAI == nullptr)
			return;

		EnemyAI->SetIsDead();
		State = EnemyState::DEAD;
		IsDead = true;

		OnEnemyDeadCheck.Broadcast();
		Dead();
	});
}

void ATODEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ATODEnemy::OnAttackCheck()
{
	if (AttackTrigger == nullptr)
		return;

	IsAttacking = true;
	AttackTrigger->SetGenerateOverlapEvents(true);
}

void ATODEnemy::OnAttackCheckEnd()
{
	if (AttackTrigger == nullptr)
		return;

	IsAttacking = false;
	AttackTrigger->SetGenerateOverlapEvents(false);
}

void ATODEnemy::AttackCoolDownTimerStart()
{
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATODEnemy::AttackCoolDownTimer, NormalAttackCoolDownTime, false);
}

void ATODEnemy::AttackCoolDownTimer()
{
	IsCanAttack = true;
}

void ATODEnemy::Dead()
{
	SetActorEnableCollision(false);
	HPBarWidget->SetHiddenInGame(true);

	if (DeadEffect != nullptr)
	{
		HitEffect->OnSystemFinished.AddDynamic(this, &ATODEnemy::OnDeadEffectFinished);
		HitEffect->SetTemplate(DeadEffect);

		FVector effectLoc = GetMesh()->GetSocketLocation("Impact");
		HitEffect->SetWorldLocation(effectLoc);
		HitEffect->Activate(true);

		GetMesh()->SetVisibility(false);
	}
}

void ATODEnemy::OnAttackTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATODCharacter* Player = Cast<ATODCharacter>(OtherActor);

	if (Player != nullptr)
	{
		print(OtherActor->GetName());

		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(EnemyStat->GetAttack(), DamageEvent, GetController(), this);

		FVector effectLoc = Player->GetMesh()->GetSocketLocation("Impact");
		StartHitEffect(effectLoc);

		// 넉백 효과 임시 적용
		//if (!IsAttacking)
		{
			if(Player->GetMovementComponent()->IsFalling())
				Player->GetCharacterMovement()->AddImpulse(this->GetActorForwardVector() * 100.0f, true);
			else
				Player->GetCharacterMovement()->AddImpulse(this->GetActorForwardVector() * 1000.0f, true);
		}

		// 플레이어 타격 후 바로 비활성화
		AttackTrigger->SetGenerateOverlapEvents(false);
	}
}

void ATODEnemy::OnDeadEffectFinished(class UParticleSystemComponent* PSystem)
{
	Destroy();
}