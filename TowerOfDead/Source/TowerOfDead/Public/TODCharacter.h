#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Character.h"
#include "TODCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHardAttackCastDelegate);

UCLASS()
class TOWEROFDEAD_API ATODCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATODCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;

	void SetControl();
	void SetPlayerDead();

	UFUNCTION(BlueprintCallable)
	void SetCharacterMove(bool isMoveing);

	void Attack();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();

	void SetCanAttackDamage(bool bCan){ WeaponTrigger->SetGenerateOverlapEvents(bCan); }

	void HardAttack();
	void HardAttackCheck();
	void HardAttackCoolDownTimer();

	FOnHardAttackCastDelegate OnHardAttackCast;
	float GetHardAttackRatio() { return CastTime / HardAttackTime; }

	void SpecialAttack();
	void SpecialAttackCatch();
	void SpecialAttackEnd();
	void SpecialAttackCatchTimer();
	void SpecialAttackCoolDownTimer();

	void HardAndSpecialAttackHitCheck(int32 AttackType, float Range);

	UFUNCTION()
	void OnWewaponTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UTODCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Attack)
	class UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Weapon)
	class UCapsuleComponent* WeaponTrigger;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* SwordEffect;

private:
	class UTODAnimInstance* Anim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDead;

	// 콤보 공격 ------------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttaking;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;
	//-------------------------------------------------------

	// 강공격 --------------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsHardAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanHardAttack;

	float CastTime = 0.0f;
	float HardAttackTime = 2.0f;
	float HardAttackCoolDownTime = 5.0f;
	FTimerHandle HardAttackTimerHandle;
	// -----------------------------------------------------

	// 특수 공격 --------------------------------------------
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsSpecialAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanSpecialAttack;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanSpecialCatch;

	float SpecialAttackCoolDownTime = 5.0f;

	FTimerHandle SpecialAttackTimerHandle;
	FTimerHandle SpecialCatchTimerHandle;

	bool IsWeaponFall = false;
	float FalldeltaTime = 0.0f;
	// -------------------------------------------------------

};
