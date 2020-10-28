#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Actor.h"
#include "TODMeteor.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATODMeteor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void SetMeteor(float damage);
	void MeteorDropStart();
	void MeteorDropEnd();

	void MeteorExplodeCheckEnd();

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* BaseEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* MeteorHitEffect;

private:
	UFUNCTION()
	void OnMeteorTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnExplodeTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	class USphereComponent* MeteorTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	class USphereComponent* ExplodeTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	float MeteorDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	class UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* MeteorEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle MeteorDropTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	FTimerHandle ExplodeTimerHandle;
};
