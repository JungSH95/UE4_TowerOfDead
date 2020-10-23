#include "Enemy/TODGruxAIAnimInstance.h"
#include "Enemy/TODEnemyGrux.h"

UTODGruxAIAnimInstance::UTODGruxAIAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_1(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_Attack1_Montage.Grux_Attack1_Montage"));
	if (ATTACK_MONTAGE_1.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_1.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE_2(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_Attack2_Montage.Grux_Attack2_Montage"));
	if (ATTACK_MONTAGE_2.Succeeded())
		ArrAttackMontage.Add(ATTACK_MONTAGE_2.Object);

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DOUBLEATTACK_MONTAGE(TEXT("/Game/ParagonGrux/Characters/Heroes/Grux/Animations/Montage/Grux_DoubleAttack_Montage.Grux_DoubleAttack_Montage"));
	if (DOUBLEATTACK_MONTAGE.Succeeded())
		DoubleAttackMontage = DOUBLEATTACK_MONTAGE.Object;
}

void UTODGruxAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

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
	TODLOG_S(Warning);

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