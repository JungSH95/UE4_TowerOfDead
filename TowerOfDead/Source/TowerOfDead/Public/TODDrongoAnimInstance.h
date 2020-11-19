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

	void PlayAttackMontage();

	void PlayBazookzEquipMontage();

	void PlayGrenadePrepMontage();

public:
	UAnimMontage* GetBazookaEquipMontage() { return BazookaEquipMontage; }
	UAnimMontage* GetGrenadePrepMontage() { return GrenadePrepMontage; }

	void SetIsBazooka(bool bazooka) { IsBazooka = bazooka; }
	bool GetIsBazooka() { return IsBazooka; }

	void SetIsGrenade(bool grenade) { IsGrenade = grenade; }
	bool GetIsGrenade() { return IsGrenade; }

private:
	//UFUNCTION()
	//void AnimNotify_SetCanAttack();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsBazooka;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BazookaEquipMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsGrenade;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* GrenadePrepMontage;
};
