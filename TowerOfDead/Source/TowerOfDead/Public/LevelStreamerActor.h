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

public:
	void SetNextLevel(FName stagelevel);
	void SetNextLevelEvent(bool isEvent) { OverlapVolume->SetGenerateOverlapEvents(isEvent); }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* OverlapVolume;
};
