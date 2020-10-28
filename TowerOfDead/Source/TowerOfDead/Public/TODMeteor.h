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

	void StartMeteor();

	UFUNCTION()
	void OnMeteorTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystem* MeteorHitEffect;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Skill, Meta = (AllowPrivateAccess = true))
	class USphereComponent* MeteorTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* MeteorEffect;
};
