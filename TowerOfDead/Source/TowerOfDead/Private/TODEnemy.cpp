#include "TODEnemy.h"
#include "TODEnemyAIController.h"
#include "TODAIAnimInstance.h"

ATODEnemy::ATODEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ATODEnemyAIController::StaticClass();
	// 레벨에 배치하거나 새롭게 생성되는 Enemy마다 ANNSEnemyAIController액터 생성 및 지배를 받는다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODEnemy"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	State = EnemyState::PEACE;
	AttackRange = 300.0f;
	EffectiveRange = AttackRange + 100.0f;

	IsCanAttack = true;
	IsDead = false;
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
}

void ATODEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

	ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(GetController());
	if (EnemyAI == nullptr)
		return;

	EnemyAI->SetIsAttaking(false);
	AnimInstance->NowMontage = nullptr;
	IsCanAttack = true;
}