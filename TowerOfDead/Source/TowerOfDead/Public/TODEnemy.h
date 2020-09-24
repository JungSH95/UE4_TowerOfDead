#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Character.h"
#include "TODEnemy.generated.h"

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

	float GetAttackRange() { return AttackRange; }
	float GetEffectiveRange() { return EffectiveRange; }

	bool GetIsCanAttack() { return IsCanAttack; }

	void Attack();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackCoolDownTime();

	bool GetIsDead() { return IsDead; }

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
};
