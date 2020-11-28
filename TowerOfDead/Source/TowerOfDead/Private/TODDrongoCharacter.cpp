#include "TODDrongoCharacter.h"
#include "TODDrongoAnimInstance.h"
#include "DrawDebugHelpers.h"

ATODDrongoCharacter::ATODDrongoCharacter()
{
	// �⺻ ĳ���� �޽�
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

// ��� ��� �� ȣ���Ͽ� ���� ���� ���·�
void ATODDrongoCharacter::SetIsCanAttack(bool attack)
{
	IsCanAttack = attack;

	// Bazooka Attack�� �����ߴٸ�
	if (IsBazookaAttacking)
		IsBazookaAttacking = false;
}

void ATODDrongoCharacter::Attack()
{
	if (Anim == nullptr || !IsCanAttack || IsGrenadeAttacking)
		return;

	// ����ī ���� ���� ����
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
		// ���� ���� ǥ�� (��Į)
		DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 2.0f);

		// �� ��ġ�� �ѱ����� �߻� ��Ŵ
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
	/* ���ϴ� �������� ������ ȣ��ǹǷ� ��� X
	// ����ī �߻簡 ������
	if (Cast<UTODDrongoAnimInstance>(Anim)->GetBazookaFireMontage() == Montage)
	{
		//IsCanAttack = true;
		//IsBazookaAttacking = false;
	}
	*/
}

// ����ź? ������
void ATODDrongoCharacter::ActionMouseRight()
{
	// ����ī ��� ������̶�� �н�
	if (IsBazookaAttacking)
		return;

	// ��Ÿ�� ���� �� �̶��
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
	// ����ź�� ��� ���� �ʴٸ�
	if (!IsGrenadeAttacking)
		return;
	
	// ���콺 Ŀ�� �������� ����ź ������ (��Ÿ� ����) == Kwang�� Į ������
	Cast<UTODDrongoAnimInstance>(Anim)->PlayGrenadeThrowMontage();

	IsGrenadeAttacking = false;
}

void ATODDrongoCharacter::GrenadeDelayTimer()
{
	print(FString::Printf(TEXT("Is Can Grenade Skill")));
	IsCanGrenade = true;
}

// ����ī ��
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
	// ���콺 Ŀ�� �������� ����ī �߻� (��ź�� ����?)
	Cast<UTODDrongoAnimInstance>(Anim)->PlayBazookFireMontage();

	IsCanBazookaFireAttack = false;

	// �˹� and ����
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