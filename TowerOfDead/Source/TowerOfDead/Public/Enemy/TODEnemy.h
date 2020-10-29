#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Character.h"
#include "TODEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyDeadDelegate);

UCLASS()
class TOWEROFDEAD_API ATODEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ATODEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EnemyState GetState() { return State; }
	void SetState(EnemyState newState) { State = newState; }
	void SetIsCanAttack(bool isCanAttack) { IsCanAttack = isCanAttack; }

	float GetAttackRange() { return AttackRange; }
	float GetEffectiveRange() { return EffectiveRange; }
	bool GetIsCanAttack() { return IsCanAttack; }
	bool GetIsAttacking() { return IsAttacking; }
	bool GetIsDead() { return IsDead; }
	class UBoxComponent* GetAttackTrigger() { return AttackTrigger;	}
	
	virtual void Attack() PURE_VIRTUAL(ATODEnemy::Attack, return;);
	virtual void OutRangeAttack(float dis) PURE_VIRTUAL(ATODEnemy::OutRangeAttack, return;);
	virtual bool GetIsCanOutRangeAttack() PURE_VIRTUAL(ATODEnemy::OutRangeAttack, return false;);

	virtual void StartHitEffect(FVector pos) PURE_VIRTUAL(ATODEnemy::StartHitEffect, return;);
	virtual void StartAllSkillCoolDown() PURE_VIRTUAL(ATODEnemy::StartAllSkillCoolDown, return;);
	virtual void LevelStartMontage() PURE_VIRTUAL(ATODEnemy::LevelStartMontage, return;);

	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) PURE_VIRTUAL(ATODEnemy::OnAttackMontageEnded, return;);

	void OnAttackCheck();
	void OnAttackCheckEnd();
	void AttackCoolDownTimerStart();
	void AttackCoolDownTimer();

	void Dead();

	FOnEnemyDeadDelegate OnEnemyDeadCheck;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UTODEnemyStatComponent* EnemyStat;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* HitEffect;

private:
	UFUNCTION()
	void OnAttackTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EnemyState State;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float EffectiveRange;

	// 공격 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	// 공격 가능 유무
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	// 일반 공격 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float NormalAttackCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FTimerHandle AttackTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UBoxComponent* AttackTrigger;
};