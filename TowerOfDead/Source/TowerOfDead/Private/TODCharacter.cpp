#include "TODCharacter.h"
#include "TODCharacterStatComponent.h"
#include "TODAnimInstance.h"
#include "TODPlayerController.h"
#include "TODPlayerState.h"
#include "TODGameMode.h"
#include "TODUserWidget.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/SkeletalMeshSocket.h"

ATODCharacter::ATODCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CharacterStat = CreateDefaultSubobject<UTODCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	SpringArm = CreateDefaultSubobject <USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject <UCameraComponent>(TEXT("CAMERA"));
	Decal = CreateDefaultSubobject <UDecalComponent>(TEXT("DECAL"));
	WeaponTrigger = CreateDefaultSubobject<UCapsuleComponent>(TEXT("WEAPONTRIGGER"));
	SwordEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->bEnableCameraRotationLag = true;
	Camera->SetupAttachment(SpringArm);

	Decal->SetupAttachment(RootComponent);
	Decal->SetVisibility(false);
	Decal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	GetMesh()->bReceivesDecals = false;

	WeaponTrigger->SetupAttachment(GetMesh(), FName(TEXT("Weapon")));
	WeaponTrigger->SetGenerateOverlapEvents(false);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("TODCharacter"));


	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	HitEffect->SetupAttachment(RootComponent);
	HitEffect->bAutoActivate = false;

	SetControl();
	
	IsDead = false;

	// ȸ�� or ���� Object
	IsSoulRecovery = false;
	IsCanStopSoulRecovery = false;
}

void ATODCharacter::BeginPlay()
{
	Super::BeginPlay();

	TODLOG_S(Warning);

	ATODPlayerController* playerController = Cast<ATODPlayerController>(GetController());
	if (playerController != nullptr)
	{
		PlayerController = playerController;
		PlayerController->GetUserHUDWidget()->BindCharacterStatClass(CharacterStat);

		SetPlayerStart(true);
	}

	auto TODPlayerState = Cast<ATODPlayerState>(GetPlayerState());
	if (TODPlayerState != nullptr)
	{
		CharacterStat->SetNewCharacterStat(
			TODPlayerState->GetHPLevel(),
			TODPlayerState->GetATKLevel(),
			TODPlayerState->GetDEFLevel(),
			GetWorld()
		);
	}
}

void ATODCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead)
	{
		if (SpringArm->TargetArmLength <= 1000.0f)
			SpringArm->TargetArmLength += (250.0f * GetWorld()->DeltaTimeSeconds);

		return;
	}
}

void ATODCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TODLOG_S(Warning);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		Anim->SetIsDead();
		SetActorEnableCollision(false);

		SetPlayerDead();
	});

	WeaponTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODCharacter::OnWewaponTriggerOverlap);
}

void ATODCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATODCharacter::Jump);
}

float ATODCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

void ATODCharacter::SetControl()
{
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->TargetOffset = FVector(0.0f, 0.0f, 70.0f);

	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;

	// ī�޶� ��ġ �� ���� : ī�޶��� �ε巯�� �������� ���ؼ�
	SpringArm->bEnableCameraLag = true;

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
	GetCharacterMovement()->JumpZVelocity = 400.0f;
}

void ATODCharacter::SetPlayerStart(bool start)
{
	if (PlayerController == nullptr)
		return;

	PlayerController->SetCanInputAction(start);
}

void ATODCharacter::SetPlayerDead()
{
	// Ư�� ���� ���� ����(Decal)
	Decal->SetVisibility(false);

	//IsSpecialAttacking = false;

	// ������ ���� ����
	//IsHardAttacking = false;
	PlayerController->GetUserHUDWidget()->SetVisibleCast(false);

	// �̵� �Ұ�
	SetCharacterMove(false);
	IsDead = true;
	// Ű �Է� �Ұ�
	PlayerController->SetCanInputAction(false);
}

void ATODCharacter::SetCharacterMove(bool isMoveing)
{
	if (IsDead)
		isMoveing = false;

	PlayerController = Cast<ATODPlayerController>(GetController());
	if (PlayerController != nullptr)
		PlayerController->SetIsMove(isMoveing);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = isMoveing;
}

void ATODCharacter::SetBossEnemyStatBind(class UTODEnemyStatComponent* enemyStat)
{
	if (enemyStat == nullptr)
		return;

	PlayerController->GetUserHUDWidget()->BindBossEnemyStateClass(enemyStat);
}

void ATODCharacter::SoulRecovery()
{
	if (IsSoulRecovery)
		return;

	PlayerController->GetUserHUDWidget()->SetVisibleLevelUp(true);
	FInputModeGameAndUI GameAndUIInputMode;
	PlayerController->SetInputMode(GameAndUIInputMode);
	PlayerController->bShowMouseCursor = true;

	SetIsSoulRecovery(true);	

	// ���Ⱑ ������ �ִٸ� ĳġ ���� �� ȸ��
	//if (Anim->GetIsSpecialTarget())
	//	SpecialAttackCatch();

	// �ִϸ��̼� Flag On
	Anim->SetIsSoulRecovery(true);
	// ü�� ȸ��
	CharacterStat->RecoveryHP();
}

void ATODCharacter::SoulRecoveryEnd()
{
	// SoulRecovery ���� �ƴҶ� ������ X
	if (!IsSoulRecovery || !IsCanStopSoulRecovery)
		return;

	PlayerController->GetUserHUDWidget()->SetVisibleLevelUp(false);
	FInputModeGameOnly GameInputMode;
	PlayerController->SetInputMode(GameInputMode);
	PlayerController->bShowMouseCursor = false;

	Anim->SetIsSoulRecovery(false);
}

void ATODCharacter::SetIsSoulRecovery(bool isSoulRecovery)
{
	// Soul Recovery ����
	if (isSoulRecovery)
	{
		// �̵� �Ұ�
		SetCharacterMove(false);
		// Ű �Է� �Ұ�
		PlayerController->SetCanInputAction(false);
	}
	// Soul Recovery ����
	else
	{
		// �̵� ����
		SetCharacterMove(true);
		// Ű �Է� ����
		PlayerController->SetCanInputAction(true);
	}

	IsSoulRecovery = isSoulRecovery;
	IsCanStopSoulRecovery = false;
}

/*

void ATODCharacter::SpecialAttackCatch()
{
	// ���� �ޱ�
	Anim->PlayCatchMontage();
	
	// ��Ÿ�� ����
	GetWorldTimerManager().SetTimer(SpecialAttackTimerHandle, this,
		&ATODCharacter::SpecialAttackCoolDownTimer, SpecialAttackCoolDownTime, false);
}

void ATODCharacter::SpecialAttackCatchTimer()
{
	IsCanSpecialCatch = true;
}

void ATODCharacter::SpecialAttackCoolDownTimer()
{
	IsCanSpecialAttack = true;
}
*/

void ATODCharacter::HardAndSpecialAttackHitCheck(int32 AttackType, float Range)
{
	// Attack Type Check
	FVector VTarget;
	switch (AttackType)
	{
	// Hard Attack
	case 0:
		VTarget = GetActorLocation();
		break;
	// Special Attack
	case 1:
		VTarget = Anim->GetTargetPoint();
		break;
	default:
		VTarget = GetActorLocation();
		break;
	}

	// �ش� ���� ���� ������
	TArray<FOverlapResult> OverlapActors;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapActors,
		VTarget,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(Range),
		CollisionQueryParam
	);

	DrawDebugSphere(GetWorld(), VTarget, Range, 16, FColor::Red, false, 10.0f);

	// Hard/Special Attack Channels (ECC_GameTraceChannel4)�� �ɸ��� ������Ʈ
	if (bResult)
	{
		for (auto OverlapActor : OverlapActors)
		{
			ATODEnemy* Enemy = Cast<ATODEnemy>(OverlapActor.GetActor());
			if (Enemy != nullptr)
			{
				bool isCanDamage = false;

				// Hard Attack : ���� 180���� ������ �ִ� �� ����
				if (AttackType == 0)
				{
					float Angle = FVector::DotProduct(GetActorForwardVector(), Enemy->GetActorLocation() - GetActorLocation());
					
					// ����(180��)�� �ִ� Enemy���Ը� ���� ó��
					if (Angle >= 0.0f)
						isCanDamage = true;
				}
				else if (AttackType == 1)
				{
					isCanDamage = true;
				}

				// ������ ������ Enemy
				if (isCanDamage)
				{
					FDamageEvent DamageEvent;
					Enemy->TakeDamage(15.0f, DamageEvent, GetController(), this);

					DrawDebugLine(GetWorld(), VTarget, Enemy->GetActorLocation(),
						FColor::Blue, false, 5.0f);
				}
			}
		}
	}
}

void ATODCharacter::OnWewaponTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult)
{
	ATODEnemy* Enemy = Cast<ATODEnemy>(OtherActor);
	if (Enemy != nullptr)
	{
		// ���ݹ��� ���� Ȯ��
		//if (HitEnemyCheck(Enemy))
		{
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);

			FVector effectLoc = Enemy->GetMesh()->GetSocketLocation("Impact");
			HitEffect->SetWorldLocation(effectLoc);
			HitEffect->Activate(true);
		}
	}
}