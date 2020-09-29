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

	UFUNCTION()
	void SetCharacterMove(bool isMoveing);

	void Attack();
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();

	void HardAttack();
	void HardAttackCheck();
	void HardAttackCoolDownTimer();

	FOnHardAttackCastDelegate OnHardAttackCast;
	float GetHardAttackRatio() { return CastTime / HardAttackTime; }

	void SpecialAttack();
	void SpecialAttackEnd();
	void SpecialAttackCatch();
	void SpecialAttackCoolDownTimer();

	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Attack)
	class UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* SwordEffect;

private:
	class UTODAnimInstance* Anim;

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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsHardAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanHardAttack;

	float CastTime = 0.0f;
	float HardAttackTime = 2.0f;
	float HardAttackCoolDownTime = 5.0f;
	FTimerHandle HardAttackTimerHandle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsSpecialttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsCanSpecialttack;

	float SpecialAttackCoolDownTime = 5.0f;
	FTimerHandle SpecialAttackTimerHandle;

	bool IsWeaponFall = false;
	float deltaTime = 0.0f;

	/*
	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UMaterial* InVisibleWeaponMaterial;

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	UMaterial* VisibleWeaponMaterial;
	*/
};
