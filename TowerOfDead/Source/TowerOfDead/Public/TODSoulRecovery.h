#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Actor.h"
#include "TODSoulRecovery.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODSoulRecovery : public AActor
{
	GENERATED_BODY()
	
public:	
	ATODSoulRecovery();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UFUNCTION()
	void OnPlayerCheckTriggerBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerCheckTriggerEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* KeyWidget;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* BaseGroundObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Effect, Meta = (AllowPrivateAccess = true))
	UParticleSystemComponent* BaseEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Trigger, Meta = (AllowPrivateAccess = true))
	class USphereComponent* PlayerCheckTrigger;
};
