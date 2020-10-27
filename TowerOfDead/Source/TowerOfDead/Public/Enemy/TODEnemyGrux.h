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
	virtual void PostInitializeComponents() override;

	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	bool GetIsDashSkilling() { return IsDashSKilling; }
	bool GetIsCanOutRangeAttack();

public:
	void StartAllSkillCoolDown();

	void Attack();

	void DoubleAttackHitCheck();
	void DoubleAttackHitCheckEnd();

	void SkillDelayTimer();

	// ���׿� or ���� ��ȯ�� ���
	void RandomPointInit(int count);

	void MeteorSkill();
	void MeteorSkillCoolDownTimer();

	void DashSkill();
	void OnDashSkillOverlap();

	void DashSkillEndTimer();
	void DashSkillCoolDownTimer();
	void StunEnd();

	void EnemySpawnSkill();
	void EnemySpawnSkillCoolDownTimer();

	// BTTask_Attack���� ����, ������ ��ų �Ǻ� �� ���
	void OutRangeAttack(float dis);

	// ���ݿ� �´� ����Ʈ�� ����
	void StartHitEffect(FVector pos);

	UFUNCTION()
	void OnDashTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* AttackHitEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* DoubleAttackHitEffect;
	
	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* DashSkillHitEffect;

	// ��ȯ�� Actor Test��
	UPROPERTY(EditAnywhere, Category = Point)
	TSubclassOf<AActor> ActorToSpawn;


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDoubleAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle SkillDelayTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	float SkillDelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	class USphereComponent* DashTrigger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	float DashSkillCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle DashSkillTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle DashSkillEndTimerHandle;
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
