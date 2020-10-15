#pragma once

#include "TowerOfDead.h"
#include "GameFramework/GameModeBase.h"
#include "TODGameMode.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATODGameMode();

	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	UFUNCTION(BlueprintCallable)
	void SetIsSequencePlaying(bool isPlaying) { IsSequencePlaying = isPlaying; }
	UFUNCTION(BlueprintCallable)
	bool GetIsSequencePlaying() { return IsSequencePlaying; }

	void AddLoadLevel(FName name) { ArrLoadLevel.Add(name); }
	bool IsLoadLevel(FName name);

	void AddArrStageManager(class ATODStageManager* manager) { ArrStageManager.Add(manager); }
	ATODStageManager* GetStageManager(FName name);

	void PlayFadeIn();
	void PlayFadeOut();

private:
	UPROPERTY(EditAnywhere)
	class ALevelSequenceActor* FadeInSequence;

	UPROPERTY(EditAnywhere)
	class ALevelSequenceActor* FadeOutSequence;

	UPROPERTY(EditAnywhere)
	bool IsSequencePlaying;

	UPROPERTY(EditAnywhere)
	TArray<FName> ArrLoadLevel;

	UPROPERTY(EditAnywhere)
	TArray<class ATODStageManager*> ArrStageManager;
};
