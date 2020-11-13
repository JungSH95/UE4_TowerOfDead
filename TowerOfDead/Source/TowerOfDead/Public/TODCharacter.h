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
	UFUNCTION(BlueprintCallable)
	void SetPlayerStart(bool start);
	void SetPlayerDead();

	UFUNCTION(BlueprintCallable)
	void SetCharacterMove(bool isMoveing);

	void SetBossEnemyStatBind(class UTODEnemyStatComponent* enemyStat);

	void SoulRecovery();
	void SoulRecoveryEnd();

	void SetIsSoulRecovery(bool isSoulRecovery);

	void SetIsCanStopSoulRecovery(bool isCanStopSoulRecovery) { IsCanStopSoulRecovery = isCanStopSoulRecovery; }
	bool GetIsCanStopSoulRecovery() { return IsCanStopSoulRecovery; }
	
	bool GetIsDead() { return IsDead; }

	// 캐스트에 사용하는 것들 모두 사용할 수 있도록
	virtual float GetCastSkillRatio() PURE_VIRTUAL(ATODCharacter::GetCastSkillRatio, return 0.0f;);

	virtual void Attack() PURE_VIRTUAL(ATODCharacter::Attack, return;);
	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) PURE_VIRTUAL(ATODCharacter::OnAttackMontageEnded, return;);
	
	//void SetCanAttack() { CanAttack = true; };
	
	void SetCanAttackDamage(bool bCan){ WeaponTrigger->SetGenerateOverlapEvents(bCan); }

	virtual void ActionMouseRight() PURE_VIRTUAL(ATODCharacter::ActionMouseRight, return;);
	virtual void ActionMouseRightEnd() PURE_VIRTUAL(ATODCharacter::ActionMouseRightEnd, return;);
	/*
	void HardAttack();
	void HardAttackCheck();
	void HardAttackEnd();
	void HardAttackCoolDownTimer();
	*/

	virtual void ActionKeyboardR() PURE_VIRTUAL(ATODCharacter::ActionKeyboardR, return;);
	virtual void ActionKeyboardREnd() PURE_VIRTUAL(ATODCharacter::ActionKeyboardREnd, return;);
	/*void SpecialAttack();
	void SpecialAttackCatch();
	void SpecialAttackEnd();
	void SpecialAttackCatchTimer();
	void SpecialAttackCoolDownTimer();*/

	void HardAndSpecialAttackHitCheck(int32 AttackType, float Range);

	FOnHardAttackCastDelegate OnHardAttackCast;

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

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* HitEffect;

protected:
	class UTODAnimInstance* Anim;

	UPROPERTY()
	class ATODPlayerController* PlayerController;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDead;	

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Recovery, Meta = (AllowPrivateAccess = true))
	bool IsSoulRecovery;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Recovery, Meta = (AllowPrivateAccess = true))
	bool IsCanStopSoulRecovery;
};
