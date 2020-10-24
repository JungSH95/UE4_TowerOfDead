#pragma once

#include "TowerOfDead.h"
#include "TODEnemy.h"
#include "TODEnemyGrux.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemyGrux : public ATODEnemy
{
	GENERATED_BODY()

public:
	ATODEnemyGrux();

	virtual void Tick(float DeltaTime) override;

public:
	void Attack();

	void DoubleAttackHitCheck();
	void DoubleAttackHitCheckEnd();

	// ���׿� or ���� ��ȯ�� ���
	void RandomPointInit(int count);

	void MeteorSkillCoolDownTimer();

	void DashSkill();
	void DashSkillCoolDownTimer();

	void EnemySpawnSkillCoolDownTimer();

	// BTTask_Attack���� ����, ������ ��ų �Ǻ� �� ���
	bool OutRangeAttack(float dis);

	// ���ݿ� �´� ����Ʈ�� ����
	void StartHitEffect(FVector pos);

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* AttackHitEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* DoubleAttackHitEffect;

	// ��ȯ�� Actor Test��
	UPROPERTY(EditAnywhere, Category = Point)
	TSubclassOf<AActor> ActorToSpawn;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDoubleAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	TArray<FVector> RandomPoint;
	
	// ���׿� ��� ------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	bool IsCanMeteorSKill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	float MeteorSkillCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle MeteorSkillTimerHandle;
	// -------------------------------------

	// ���� ��� ------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	bool IsCanDashSKill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	bool IsDashSKilling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	float DashSkillCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle DashSkillTimerHandle;
	// -------------------------------------

	// ���� ��ȯ ��� ------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	bool IsCanEnemySpawnSKill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	float EnemySpawnSkillCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle EnemySpawnSkillTimerHandle;
	// -------------------------------------
};
