#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Actor.h"
#include "LevelStreamerActor.generated.h"

UCLASS()
class TOWEROFDEAD_API ALevelStreamerActor : public AActor
{
	GENERATED_BODY()

public:
	ALevelStreamerActor();


protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditAnywhere)
	FName LevelToLoad;

	UPROPERTY(EditAnywhere)
	FName NowLevel;

public:
	void SetNextLevel(FName stagelevel);
	void SetNextLevelEvent(bool isEvent) { OverlapVolume->SetGenerateOverlapEvents(isEvent); }
	UFUNCTION()
	void SetNowLevel(ATODStageManager* manager, FName level);

	void SetPortalEffectActive(bool isActive);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OverlapVolume;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* PortalEffect;

	UPROPERTY()
	ATODStageManager* StageManager;
};
