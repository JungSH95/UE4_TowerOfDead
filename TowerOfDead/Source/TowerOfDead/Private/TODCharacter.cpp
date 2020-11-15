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

	// 회복 or 성장 Object
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

	// 카메라 위치 렉 설정 : 카메라의 부드러운 움직임을 위해서
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
	// 특수 공격 관련 종료(Decal)
	Decal->SetVisibility(false);

	//IsSpecialAttacking = false;

	// 강공격 관련 종료
	//IsHardAttacking = false;
	PlayerController->GetUserHUDWidget()->SetVisibleCast(false);

	// 이동 불가
	SetCharacterMove(false);
	IsDead = true;
	// 키 입력 불가
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

	// 무기가 던져져 있다면 캐치 실행 후 회복
	//if (Anim->GetIsSpecialTarget())
	//	SpecialAttackCatch();

	// 애니메이션 Flag On
	Anim->SetIsSoulRecovery(true);
	// 체력 회복
	CharacterStat->RecoveryHP();
}

void ATODCharacter::SoulRecoveryEnd()
{
	// SoulRecovery 중이 아닐때 들어오면 X
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
	// Soul Recovery 시작
	if (isSoulRecovery)
	{
		// 이동 불가
		SetCharacterMove(false);
		// 키 입력 불가
		PlayerController->SetCanInputAction(false);
	}
	// Soul Recovery 종료
	else
	{
		// 이동 가능
		SetCharacterMove(true);
		// 키 입력 가능
		PlayerController->SetCanInputAction(true);
	}

	IsSoulRecovery = isSoulRecovery;
	IsCanStopSoulRecovery = false;
}

/*

void ATODCharacter::SpecialAttackCatch()
{
	// 무기 받기
	Anim->PlayCatchMontage();
	
	// 쿨타임 진행
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

	// 해당 범위 몬스터 얻어오기
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

	// Hard/Special Attack Channels (ECC_GameTraceChannel4)에 걸리는 오브젝트
	if (bResult)
	{
		for (auto OverlapActor : OverlapActors)
		{
			ATODEnemy* Enemy = Cast<ATODEnemy>(OverlapActor.GetActor());
			if (Enemy != nullptr)
			{
				bool isCanDamage = false;

				// Hard Attack : 전방 180도의 범위에 있는 적 공격
				if (AttackType == 0)
				{
					float Angle = FVector::DotProduct(GetActorForwardVector(), Enemy->GetActorLocation() - GetActorLocation());
					
					// 정면(180도)에 있는 Enemy에게만 공격 처리
					if (Angle >= 0.0f)
						isCanDamage = true;
				}
				else if (AttackType == 1)
				{
					isCanDamage = true;
				}

				// 공격이 가능한 Enemy
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
		// 공격받은 몬스터 확인
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