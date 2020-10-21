#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "Enemy/TODEnemyAIController.h"
#include "Enemy/TODAIAnimInstance.h"
#include "TODCharacter.h"
#include "TODPlayerController.h"
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

	/*static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/ParagonKwang/FX/Particles/Abilities/Primary/FX/P_Kwang_Primary_Impact.P_Kwang_Primary_Impact"));
	if (P_HITEFFECT.Succeeded())
	{
		HitEffect->SetTemplate(P_HITEFFECT.Object);
		HitEffect->bAutoActivate = false;
	}*/

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

		Dead();
		OnEnemyDeadCheck.Broadcast();
	});
}

void ATODEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ATODEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance != nullptr)
	{
		if (AnimInstance->PlayHitReactMontage(0))
		{
			TODLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

			FVector effectLoc = GetMesh()->GetSocketLocation("Impact");
			HitEffect->SetWorldLocation(effectLoc);
			HitEffect->Activate(true);

			EnemyStat->SetDamage(FinalDamage);
		}
	}

	if (IsDead)
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

void ATODEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
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

		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ATODEnemy::AttackCoolDownTime, NormalAttackCoolDownTime, false);
	}
}

void ATODEnemy::OnAttackCheck()
{
	if (AttackTrigger == nullptr)
		return;

	AttackTrigger->SetGenerateOverlapEvents(true);
}

void ATODEnemy::OnAttackCheckEnd()
{
	if (AttackTrigger == nullptr)
		return;

	AttackTrigger->SetGenerateOverlapEvents(false);
}

void ATODEnemy::AttackCoolDownTime()
{
	IsCanAttack = true;
}

void ATODEnemy::Dead()
{
	SetActorEnableCollision(false);
	HPBarWidget->SetHiddenInGame(true);
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

		// 플레이어 타격 후 바로 비활성화
		AttackTrigger->SetGenerateOverlapEvents(false);
	}
}