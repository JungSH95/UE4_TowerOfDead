#include "Enemy/TODGruxAIAnimInstance.h"
#include "Enemy/TODEnemyGrux.h"

UTODGruxAIAnimInstance::UTODGruxAIAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LEVELSTART_MONTAGE(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/LevelStart_Montage.LevelStart_Montage"));
	if (LEVELSTART_MONTAGE.Succeeded())
		LevelStartMontage = LEVELSTART_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_1(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_Attack1_Montage.Grux_Attack1_Montage"));
	if (ATTACK_MONTAGE_1.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_1.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_2(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_Attack2_Montage.Grux_Attack2_Montage"));
	if (ATTACK_MONTAGE_2.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_2.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DOUBLEATTACK_MONTAGE(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_DoubleAttack_Montage.Grux_DoubleAttack_Montage"));
	if (DOUBLEATTACK_MONTAGE.Succeeded())
		DoubleAttackMontage = DOUBLEATTACK_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ENEMYSPAWNCAST_MONTAGE(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_EnemySpawn_Montage.Grux_EnemySpawn_Montage"));
	if (ENEMYSPAWNCAST_MONTAGE.Succeeded())
		EnemySpawnCastMontage = ENEMYSPAWNCAST_MONTAGE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> METEORCAST_MONTAGE(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_MeteorAttack_Montage.Grux_MeteorAttack_Montage"));
	if (METEORCAST_MONTAGE.Succeeded())
		MeteorCastMontage = METEORCAST_MONTAGE.Object;
}

void UTODGruxAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto EnemyPawn = TryGetPawnOwner();
	if (::IsValid(EnemyPawn))
	{
		ATODEnemyGrux* EnemyClass = Cast<ATODEnemyGrux>(EnemyPawn);
		if (EnemyClass != nullptr)
			IsDash = EnemyClass->GetIsDashSkilling();
	}
}

void UTODGruxAIAnimInstance::PlayAttackMontage()
{
	// 동작중인 몽타주가 없다면
	if (NowMontage == nullptr)
	{
		int MontageRandomNumber = FMath::RandRange(0, 1);
		NowMontage = ArrAttackMontage[MontageRandomNumber];
		Montage_Play(NowMontage, 0.8f);
	}
}

bool UTODGruxAIAnimInstance::PlayHitReactMontage(int dir)
{
	/*if (!Montage_IsPlaying(ArrHitReactMontage[dir]))
	{
		Montage_Play(ArrHitReactMontage[dir], 1.0f);
		return true;
	}*/

	return false;
}

void UTODGruxAIAnimInstance::PlayEnemySpawnCastMontage()
{
	// 동작중인 몽타주가 없다면
	if (NowMontage == nullptr)
	{
		Montage_Play(EnemySpawnCastMontage, 1.0f);
	}
}

void UTODGruxAIAnimInstance::PlayMeteorCastMontage()
{
	// 동작중인 몽타주가 없다면
	if (NowMontage == nullptr)
	{
		Montage_Play(MeteorCastMontage, 1.0f);
	}
}

bool UTODGruxAIAnimInstance::IsAttackMontage(UAnimMontage* montage)
{
	// Attack or DoubleAttack 시 true;
	for (int i = 0; i < ArrAttackMontage.Num(); i++)
		if (montage == ArrAttackMontage[i])
			return true;

	if (DoubleAttackMontage == montage)
		return true;

	return false;
}

void UTODGruxAIAnimInstance::AnimNotify_RandomDoubleAttack()
{
	float Rand = FMath::FRandRange(0.0f, 1.0f);
	//print(FString::Printf(TEXT("Rand : %f"), Rand));

	if (Rand >= 0.4f)
		Montage_Play(DoubleAttackMontage, 1.0f);
}

void UTODGruxAIAnimInstance::AnimNotify_DoubleAttackHitCheck()
{
	ATODEnemyGrux* Enemy = Cast<ATODEnemyGrux>(TryGetPawnOwner());
	if (Enemy != nullptr)
		Enemy->DoubleAttackHitCheck();
}

void UTODGruxAIAnimInstance::AnimNotify_DoubleAttackHitCheckEnd()
{
	ATODEnemyGrux* Enemy = Cast<ATODEnemyGrux>(TryGetPawnOwner());
	if (Enemy != nullptr)
		Enemy->DoubleAttackHitCheckEnd();
}

void UTODGruxAIAnimInstance::AnimNotify_StunEnd()
{
	ATODEnemyGrux* Enemy = Cast<ATODEnemyGrux>(TryGetPawnOwner());
	if (Enemy != nullptr)
		Enemy->StunEnd();
}