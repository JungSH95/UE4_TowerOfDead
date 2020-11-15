#include "TODKwangCharacter.h"
#include "TODCharacterStatComponent.h"
#include "TODKwangAnimInstance.h"
#include "TODPlayerController.h"
#include "TODUserWidget.h"
#include "Enemy/TODEnemy.h"
#include "Components/DecalComponent.h"
#include "DrawDebugHelpers.h"

ATODKwangCharacter::ATODKwangCharacter()
{
	// 기본 캐릭터 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood"));
	if (SK_KWANG.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_KWANG.Object);

	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM(TEXT("/Game/BluePrint/Anim_KwangPlayer.Anim_KwangPlayer_C"));
	if (KWANG_ANIM.Succeeded())
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_HITEFFECT(TEXT("/Game/ParagonKwang/FX/Particles/Abilities/Primary/FX/P_Kwang_Primary_Impact.P_Kwang_Primary_Impact"));
	if (P_HITEFFECT.Succeeded())
		HitEffect->SetTemplate(P_HITEFFECT.Object);

	// 일반 공격 설정
	IsAttacking = false;
	//CanAttack = true;
	MaxCombo = 4;
	AttackEndComboState();

	// 마우스 우클릭 (강공격)
	IsHardAttacking = false;
	IsCanHardAttack = true;

	// R 버튼 (특수 공격)
	IsSpecialAttacking = false;
	IsCanSpecialAttack = true;
	IsCanSpecialCatch = false;
}

void ATODKwangCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATODKwangCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 강공격 진행중
	if (CastTime <= HardAttackTime && IsHardAttacking)
	{
		CastTime += GetWorld()->DeltaTimeSeconds;
		OnHardAttackCast.Broadcast();

		// 실패
		if (CastTime > HardAttackTime)
		{
			if (Anim != nullptr)
				Anim->Montage_Stop(0.2f, Anim->GetCurrentActiveMontage());
			HardAttackEnd();
		}
	}

	// 특수 공격 진행중 목표 위치 설정
	if (IsSpecialAttacking)
	{
		FVector StartPos = Camera->GetComponentLocation();
		FVector EndPos = Camera->GetForwardVector() * 2000.0f + StartPos;

		FHitResult Hit;
		FCollisionQueryParams TraceParams;
		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos,
			ECollisionChannel::ECC_GameTraceChannel3, TraceParams);

		if (bHit)
		{
			// 공격 범위 표시 (데칼)
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);
			Decal->SetWorldLocation(Hit.Location);

			FVector Point = Hit.Location;
			Point.Z = 1500.0f;
			if (Anim != nullptr)
				Cast<UTODKwangAnimInstance>(Anim)->SetTargetPoint(Point);
		}
	}

	// 칼 낙하 시작
	if (IsWeaponFall && Anim != nullptr)
	{
		FalldeltaTime += GetWorld()->GetDeltaSeconds();

		// 칼 낙하 위치
		FVector StartPos = Cast<UTODKwangAnimInstance>(Anim)->GetTargetPoint();
		StartPos.Z = StartPos.Z - (FalldeltaTime * 25.0f);
		FVector TargetPos = Cast<UTODKwangAnimInstance>(Anim)->GetTargetPoint();
		TargetPos.Z = 100.0f;

		if (StartPos.Z >= 1100.0f && StartPos.Z <= 1200.0f)
			SwordEffect->Activate(true);

		if (StartPos.Z <= 100.0f)
		{
			FalldeltaTime = 0.0f;
			IsWeaponFall = false;

			HardAndSpecialAttackHitCheck(1, 200.0f);

			GetWorldTimerManager().SetTimer(SpecialCatchTimerHandle, this,
				&ATODKwangCharacter::SpecialAttackCatchTimer, 1.0f, false);
		}

		Cast<UTODKwangAnimInstance>(Anim)->SetTargetPoint(StartPos);
	}
}

void ATODKwangCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Anim = Cast<UTODKwangAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr)
	{
		Anim->OnMontageEnded.AddDynamic(this, &ATODKwangCharacter::OnAttackMontageEnded);
		Cast<UTODKwangAnimInstance>(Anim)->OnNextAttackCheck.AddLambda([this]()->void
		{
			CanNextCombo = true;

			if (IsComboInputOn)
			{
				AttackStartComboState();
				Cast<UTODKwangAnimInstance>(Anim)->JumpToAttackMontageSection(CurrentCombo);
			}
		});

		Cast<UTODKwangAnimInstance>(Anim)->OnHardAttackEnd.AddUObject(this, &ATODKwangCharacter::HardAttackEnd);
		Cast<UTODKwangAnimInstance>(Anim)->OnHardAttackHitCheck.AddUObject(this, &ATODKwangCharacter::HardAndSpecialAttackHitCheck);
	}

	WeaponTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATODKwangCharacter::OnWewaponTriggerOverlap);
}

float ATODKwangCharacter::GetCastSkillRatio()
{
	// 사용중인 기술을 확인하여 해당 기술의 캐스트 비율을 반환 
	if (IsHardAttacking)
		return CastTime / HardAttackTime;

	return 0.0f;
}

void ATODKwangCharacter::Attack()
{
	if (Anim == nullptr)
		return;

	// 무기가 던져져 있다면 불가능 (기술 시전중일 때 불가능)
	if (IsHardAttacking || IsSpecialAttacking || !Anim->GetIsEquip())
		return;

	if (IsAttacking)
	{
		if (CanNextCombo)
		{
			AttackStartComboState();

			if (CurrentCombo > MaxCombo)
				return;

			Cast<UTODKwangAnimInstance>(Anim)->JumpToAttackMontageSection(CurrentCombo);
		}
	}
	else
	{
		AttackStartComboState();
		Anim->PlayAttackMontage();
		Cast<UTODKwangAnimInstance>(Anim)->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void ATODKwangCharacter::AttackStartComboState()
{
	ArrHitEnemyCheck.Empty();

	CanNextCombo = false;
	IsComboInputOn = false;
	CurrentCombo += 1;
}

void ATODKwangCharacter::AttackEndComboState()
{
	ArrHitEnemyCheck.Empty();

	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

bool ATODKwangCharacter::HitEnemyCheck(class ATODEnemy* enemy)
{
	bool isEnemy = ArrHitEnemyCheck.Contains(enemy);

	// 공격 처리 안한 몬스터라면
	if (!isEnemy)
	{
		ArrHitEnemyCheck.Add(enemy);
		return true;
	}
	else
		return false;
}

void ATODKwangCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 일반 공격 몽타주라면
	if (Montage == Anim->GetAttackMontage())
	{
		IsAttacking = false;
		AttackEndComboState();
	}
}

void ATODKwangCharacter::ActionMouseRight()
{
	// 강공격 가능 여부 확인
	if (IsCanHardAttack == false)
		return;

	// 무기가 X & 불가능 기술 사용 중 불가능
	if (!Anim->GetIsEquip() || IsSpecialAttacking)
		return;

	Cast<UTODKwangAnimInstance>(Anim)->PlayHardAttackMontage();
	SetCharacterMove(false);

	IsHardAttacking = true;
	IsCanHardAttack = false;

	CastTime = 0.0f;

	PlayerController->GetUserHUDWidget()->SetVisibleCast(true);
}

void ATODKwangCharacter::ActionMouseRightEnd()
{
	// 강공격 진행중이 아니라면 판정 체크할 필요가 없음
	if (IsHardAttacking == false)
		return;

	float percent = (CastTime / HardAttackTime);

	// 성공 : 다음 몽타주 재생
	if (percent >= 0.7f && percent <= 0.9f)
	{
		CastTime += 2.0f;	// Tick에서 HardAttack 안돌아가게

		Anim->Montage_SetPlayRate(Cast<UTODKwangAnimInstance>(Anim)->GetHardAttackMontage(), 1.0f);
		Anim->Montage_JumpToSection(FName(*FString::Printf(TEXT("HardAttack2"))),
			Cast<UTODKwangAnimInstance>(Anim)->GetHardAttackMontage());

		if (PlayerController != nullptr)
			PlayerController->GetUserHUDWidget()->SetVisibleCast(false);
	}
	// 실패 : 몽타주 정지 후 이동 및 점프 가능
	else
	{
		Anim->Montage_Stop(0.2f, Anim->GetCurrentActiveMontage());
		HardAttackEnd();
	}
}

void ATODKwangCharacter::HardAttackEnd()
{
	IsHardAttacking = false;
	SetCharacterMove(true);

	if (PlayerController != nullptr)
		PlayerController->GetUserHUDWidget()->SetVisibleCast(false);

	GetWorldTimerManager().SetTimer(HardAttackTimerHandle, this,
		&ATODKwangCharacter::HardAttackCoolDownTimer, HardAttackCoolDownTime, false);
}

void ATODKwangCharacter::HardAttackCoolDownTimer()
{
	print(FString::Printf(TEXT("Can Hard Attack!!")));
	IsCanHardAttack = true;
	CastTime = 0.0f;
}

void ATODKwangCharacter::ActionKeyboardR()
{
	// 공격 중 불가
	if (IsAttacking || Anim->Montage_IsPlaying(Cast<UTODKwangAnimInstance>(Anim)->GetHardAttackMontage()))
		return;
	
	// 특수 공격 가능
	if (IsCanSpecialAttack)
	{
		Decal->SetVisibility(true);

		// 위치 지정 시작(Tick)
		IsSpecialAttacking = true;
		// 기술 사용 가능 유무 (쿨타임)
		IsCanSpecialAttack = false;
		// 캐치 가능한 상태인지 
		IsCanSpecialCatch = false;

		// 세계 시간 느리게
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.4);
		if (PlayerController != nullptr)
			PlayerController->SetMouseSpeed(0.3f);

		GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = false;
		Cast<UTODKwangAnimInstance>(Anim)->SetSpecialAttacking(true);
	}
	// 특수 공격 불가능 (칼을 던지고 있는 상태, 쿨타임 진행 중)
	else
	{
		// 칼을 던지고 있는 상태일 경우 & Catch가 가능할 경우
		if (Cast<UTODKwangAnimInstance>(Anim)->GetIsSpecialTarget() && IsCanSpecialCatch)
			SpecialAttackCatch();
	}
}

void ATODKwangCharacter::ActionKeyboardREnd()
{
	// 특수 공격중이 아닌 경우
	if (IsSpecialAttacking == false)
		return;

	// 시간 원래대로
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0);
	if (PlayerController != nullptr)
		PlayerController->SetMouseSpeed(0.5f);

	Decal->SetVisibility(false);
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanJump = true;

	// 위치 지정 끝(Tick)
	IsSpecialAttacking = false;
	// 칼 낙하 시작
	IsWeaponFall = true;

	// 칼 낙하 이펙트 위치 지정
	FVector EffectPoint = Cast<UTODKwangAnimInstance>(Anim)->GetTargetPoint();
	EffectPoint.Z = 30.0f;
	SwordEffect->SetWorldLocation(EffectPoint);
	//SwordEffect->Activate(true);

	// 목표 지점에 칼 투척
	Cast<UTODKwangAnimInstance>(Anim)->PlayThrowMontage();
}

void ATODKwangCharacter::SpecialAttackCatch()
{
	IsCanSpecialCatch = false;

	// 무기 받기
	Cast<UTODKwangAnimInstance>(Anim)->PlayCatchMontage();

	// 쿨타임 진행
	GetWorldTimerManager().SetTimer(SpecialAttackTimerHandle, this,
		&ATODKwangCharacter::SpecialAttackCoolDownTimer, SpecialAttackCoolDownTime, false);
}

void ATODKwangCharacter::SpecialAttackCatchTimer()
{
	IsCanSpecialCatch = true;
}

void ATODKwangCharacter::SpecialAttackCoolDownTimer()
{
	print(FString::Printf(TEXT("Can Special Attack!!")));
	IsCanSpecialAttack = true;
}

void ATODKwangCharacter::HardAndSpecialAttackHitCheck(int32 AttackType, float Range)
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
		VTarget = Cast<UTODKwangAnimInstance>(Anim)->GetTargetPoint();
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

void ATODKwangCharacter::OnWewaponTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATODEnemy* Enemy = Cast<ATODEnemy>(OtherActor);
	if (Enemy != nullptr)
	{
		// 공격받은 몬스터 확인
		if (HitEnemyCheck(Enemy))
		{
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);

			FVector effectLoc = Enemy->GetMesh()->GetSocketLocation("Impact");
			HitEffect->SetWorldLocation(effectLoc);
			HitEffect->Activate(true);
		}
	}
}