#include "TODDrongoCharacter.h"
#include "TODDrongoAnimInstance.h"
#include "DrawDebugHelpers.h"

ATODDrongoCharacter::ATODDrongoCharacter()
{
	// 기본 캐릭터 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_DRONGO(TEXT("/Game/ParagonDrongo/Characters/Heroes/Drongo/Meshes/Drongo_GDC.Drongo_GDC"));
	if (SK_DRONGO.Succeeded())
		GetMesh()->SetSkeletalMesh(SK_DRONGO.Object);

	IsCanAttack = true;

	IsCanBazooka = true;
	IsBazookaAttacking = false;
	IsCanBazookaFireAttack = false;

	IsCanGrenade = true;
	IsGrenadeAttacking = false;
}

void ATODDrongoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGrenadeAttacking)
	{

	}
}

void ATODDrongoCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Anim = Cast<UTODDrongoAnimInstance>(GetMesh()->GetAnimInstance());
	if (Anim != nullptr)
	{
		Anim->OnMontageEnded.AddDynamic(this, &ATODDrongoCharacter::OnAttackMontageEnded);
	}
}

float ATODDrongoCharacter::GetSkillCastRatio()
{
	return 0.0f;
}

// 기술 사용 후 호출하여 공격 가능 상태로
void ATODDrongoCharacter::SetIsCanAttack(bool attack)
{
	IsCanAttack = attack;

	// Bazooka Attack을 수행했다면
	if (IsBazookaAttacking)
		IsBazookaAttacking = false;
}

void ATODDrongoCharacter::Attack()
{
	if (Anim == nullptr || !IsCanAttack || IsGrenadeAttacking)
		return;

	// 바주카 공격 가능 상태
	if (IsCanBazookaFireAttack)
	{
		IsCanAttack = false;

		BazookaFire();
		return;
	}

	SetIsBattle(true);
	IsCanAttack = false;
	Anim->PlayAttackMontage();

	FVector StartPos = Camera->GetComponentLocation();
	FVector EndPos = Camera->GetForwardVector() * 5000.0f + StartPos;

	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartPos, EndPos,
		ECollisionChannel::ECC_GameTraceChannel3, TraceParams);

	
	if (bHit)
	{
		DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Green, false, 0.5f);
		// 공격 범위 표시 (데칼)
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);

		// 이 위치로 총구에서 발사 시킴
		DrawDebugLine(GetWorld(), GetMesh()->GetSocketLocation("Muzzle_01"), Hit.ImpactPoint, FColor::Red, false, 0.5f);
	}
	else
		DrawDebugLine(GetWorld(), StartPos, EndPos, FColor::Red, false, 0.5f);


	GetWorldTimerManager().SetTimer(AttackDelayTimerHandle, this,
		&ATODDrongoCharacter::AttackDelayTime, 0.5f, false);

	GetWorldTimerManager().ClearTimer(IsBattleTimerHandle);
	GetWorldTimerManager().SetTimer(IsBattleTimerHandle, this, &ATODCharacter::BattleEnd, 5.0f, false);
}

void ATODDrongoCharacter::AttackDelayTime()
{
	IsCanAttack = true;
}

void ATODDrongoCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	/* 원하는 지점보다 빠르게 호출되므로 사용 X
	// 바주카 발사가 끝나면
	if (Cast<UTODDrongoAnimInstance>(Anim)->GetBazookaFireMontage() == Montage)
	{
		//IsCanAttack = true;
		//IsBazookaAttacking = false;
	}
	*/
}

// 수류탄? 던지기
void ATODDrongoCharacter::ActionMouseRight()
{
	// 바주카 기술 사용중이라면 패스
	if (IsBazookaAttacking)
		return;

	// 쿨타임 진행 중 이라면
	if (!IsCanGrenade)
		return;

	Cast<UTODDrongoAnimInstance>(Anim)->PlayGrenadePrepMontage();

	IsCanAttack = false;
	IsCanGrenade = false;
	IsGrenadeAttacking = true;

	GetWorldTimerManager().SetTimer(GrenadeDelayTimerHandle, this,
		&ATODDrongoCharacter::GrenadeDelayTimer, 5.0f, false);
}

void ATODDrongoCharacter::ActionMouseRightEnd()
{
	// 수류탄을 들고 있지 않다면
	if (!IsGrenadeAttacking)
		return;
	
	// 마우스 커서 지점으로 수류탄 던지기 (사거리 제한) == Kwang의 칼 던지기
	Cast<UTODDrongoAnimInstance>(Anim)->PlayGrenadeThrowMontage();

	IsGrenadeAttacking = false;
}

void ATODDrongoCharacter::GrenadeDelayTimer()
{
	print(FString::Printf(TEXT("Is Can Grenade Skill")));
	IsCanGrenade = true;
}

// 바주카 포
void ATODDrongoCharacter::ActionKeyboardR()
{
	if (IsCanBazooka)
	{
		Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookzEquipMontage();

		IsCanBazooka = false;

		IsBazookaAttacking = true;
		IsCanBazookaFireAttack = true;
	}
}

void ATODDrongoCharacter::ActionKeyboardREnd()
{
	return;
}

void ATODDrongoCharacter::BazookaFire()
{
	// 마우스 커서 방향으로 바주카 발사 (산탄총 느낌?)
	Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookFireMontage();

	IsCanBazookaFireAttack = false;

	// 넉백 and 띄우기
	FVector AddForce = GetActorForwardVector() * -500.0f + FVector(0, 0, 1) * 500.0f;
	LaunchCharacter(AddForce, true, false);

	GetWorldTimerManager().SetTimer(BazookaDelayTimerHandle, this,
		&ATODDrongoCharacter::BazookaDelayTimer, 5.0f, false);
}

void ATODDrongoCharacter::BazookaDelayTimer()
{
	print(FString::Printf(TEXT("Is Can Bazooka Skill")));
	IsCanBazooka = true;
}