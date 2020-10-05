#include "TODEnemy.h"
#include "TODEnemyAIController.h"
#include "TODAIAnimInstance.h"
#include "TODCharacter.h"

ATODEnemy::ATODEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ATODEnemyAIController::StaticClass();
	// 레벨에 배치하거나 새롭게 생성되는 Enemy마다 ANNSEnemyAIController액터 생성 및 지배를 받는다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODEnemy"));
	
	GetMesh()->bReceivesDecals = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	
	AttackTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackTriggerBox"));
	AttackTrigger->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	AttackTrigger->SetGenerateOverlapEvents(false);
	
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
		}
	}

	return FinalDamage;
}

void ATODEnemy::Attack()
{
	if (!IsCanAttack)
		return;

	auto AnimInstance = Cast<UTODAIAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
		return;

	AnimInstance->PlayAttackMontage();
	IsCanAttack = false;
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

void ATODEnemy::OnAttackTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATODCharacter* Player = Cast<ATODCharacter>(OtherActor);

	if (Player != nullptr)
	{
		print(OtherActor->GetName());
		FDamageEvent DamageEvent;
		OtherActor->TakeDamage(25.0f, DamageEvent, GetController(), this);

		// 플레이어 타격 후 바로 비활성화
		AttackTrigger->SetGenerateOverlapEvents(false);
	}
}