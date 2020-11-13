#pragma once

#include "TowerOfDead.h"
#include "TODCharacter.h"
#include "TODKwangCharacter.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODKwangCharacter : public ATODCharacter
{
	GENERATED_BODY()
	
public:
	ATODKwangCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void Attack();
	void AttackStartComboState();
	void AttackEndComboState();
	bool HitEnemyCheck(class ATODEnemy* enemy);

	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	// 공격 1번에 1번만 Damage 받을 수 있도록
	UPROPERTY()
	TArray<ATODEnemy*> ArrHitEnemyCheck;

	// 콤보 공격 ------------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//bool CanAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
	//-------------------------------------------------------
};
