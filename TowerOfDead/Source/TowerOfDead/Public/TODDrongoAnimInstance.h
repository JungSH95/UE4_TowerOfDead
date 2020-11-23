#pragma once

#include "TowerOfDead.h"
#include "TODAnimInstance.h"
#include "TODDrongoAnimInstance.generated.h"

UCLASS()
class TOWEROFDEAD_API UTODDrongoAnimInstance : public UTODAnimInstance
{
	GENERATED_BODY()
	
public:
	UTODDrongoAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

	void PlayBazookzEquipMontage();
	void PlayBazookFireMontage();
	void PlayBazookLandMontage();

	void PlayGrenadePrepMontage();
	void PlayGrenadeThrowMontage();

public:
	UAnimMontage* GetBazookaEquipMontage() { return BazookaEquipMontage; }
	UAnimMontage* GetBazookaFireMontage() { return BazookaFireMontage; }
	UAnimMontage* GetGrenadePrepMontage() { return GrenadePrepMontage; }

	void SetIsBazooka(bool bazooka) { IsBazooka = bazooka; }
	bool GetIsBazooka() { return IsBazooka; }

	void SetIsGrenade(bool grenade) { IsGrenade = grenade; }
	bool GetIsGrenade() { return IsGrenade; }

private:
	UFUNCTION()
	void AnimNotify_SetCanAttack();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsBazooka;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsBazookaFire;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BazookaEquipMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BazookaFireMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BazookaLandMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsGrenade;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* GrenadePrepMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* GrenadeThrowMontage;
};
