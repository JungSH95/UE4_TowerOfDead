#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Character.h"
#include "TODCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSkillCastDelegate);

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
	void SoulRecoveryEndAction();
	void SoulRecoveryEnd();

	void SetIsSoulRecovery(bool isSoulRecovery);

	void SetIsCanStopSoulRecovery(bool isCanStopSoulRecovery) { IsCanStopSoulRecovery = isCanStopSoulRecovery; }
	bool GetIsCanStopSoulRecovery() { return IsCanStopSoulRecovery; }
	
	void SetCanAttackDamage(bool bCan) { WeaponTrigger->SetGenerateOverlapEvents(bCan); }
	
	void SetIsBattle(bool battle) { IsBattle = battle; }
	bool GetIsBattle() { return IsBattle; }
	void BattleEnd();

	bool GetIsDead() { return IsDead; }

	// 캐스트에 사용하는 것들 모두 사용할 수 있도록
	virtual float GetSkillCastRatio() PURE_VIRTUAL(ATODCharacter::GetCastSkillRatio, return 0.0f;);

	virtual void Attack() PURE_VIRTUAL(ATODCharacter::Attack, return;);
	UFUNCTION()
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted) PURE_VIRTUAL(ATODCharacter::OnAttackMontageEnded, return;);

	virtual void ActionMouseRight() PURE_VIRTUAL(ATODCharacter::ActionMouseRight, return;);
	virtual void ActionMouseRightEnd() PURE_VIRTUAL(ATODCharacter::ActionMouseRightEnd, return;);

	virtual void ActionKeyboardR() PURE_VIRTUAL(ATODCharacter::ActionKeyboardR, return;);
	virtual void ActionKeyboardREnd() PURE_VIRTUAL(ATODCharacter::ActionKeyboardREnd, return;);

	FOnSkillCastDelegate OnSkillCast;

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
	UParticleSystemComponent* HitEffect;

protected:
	class UTODAnimInstance* Anim;

	UPROPERTY()
	class ATODPlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Battle)
	FTimerHandle IsBattleTimerHandle;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Battle, Meta = (AllowPrivateAccess = true))
	bool IsBattle;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsDead;	

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Recovery, Meta = (AllowPrivateAccess = true))
	bool IsSoulRecovery;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Recovery, Meta = (AllowPrivateAccess = true))
	bool IsCanStopSoulRecovery;
};
