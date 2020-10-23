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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	EnemyState GetState() { return State; }
	void SetState(EnemyState newState) { State = newState; }
	void SetIsCanAttack(bool isCanAttack) { IsCanAttack = isCanAttack; }

	float GetAttackRange() { return AttackRange; }
	float GetEffectiveRange() { return EffectiveRange; }
	bool GetIsCanAttack() { return IsCanAttack; }
	bool GetIsDead() { return IsDead; }

	virtual void Attack() PURE_VIRTUAL(ATODEnemy::Attack, return;);
	virtual bool OutRangeAttack() PURE_VIRTUAL(ATODEnemy::OutRangeAttack, return false;);
	//void Attack();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void OnAttackCheck();
	void OnAttackCheckEnd();
	void AttackCoolDownTime();

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

	// ��ȿ ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float EffectiveRange;

	// ���� ��Ÿ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float AttackRange;

	// ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanAttack;

	// �Ϲ� ���� ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	float NormalAttackCoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FTimerHandle AttackTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = true))
	class UBoxComponent* AttackTrigger;

	// 
};