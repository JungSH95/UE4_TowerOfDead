#pragma once

#include "TowerOfDead.h"
#include "LevelStreamerActor.h"
#include "TODEnemySpawnPoint.h"
#include "GameFramework/Actor.h"
#include "TODStageManager.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODStageManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATODStageManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 스테이지 클리어 시? 스테이지 생성 시? 다음 스테이지 설정
	void SetNextStage();
	void SetPlayerPosition();
	void InitEnemy();

	FTimerHandle StartTimerHandle;
	void StageStart();
	void StageClearCheck();
	void ReSetStage();

	UPROPERTY(EditAnywhere)
	FName LevelName;

private:
	UPROPERTY(EditAnywhere)
	class ATODGameMode* TODGameMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stage, Meta = (AllowPrivateAccess = true))
	AActor* StartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stage, Meta = (AllowPrivateAccess = true))
	bool IsClear;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stage, Meta = (AllowPrivateAccess = true))
	bool IsBattleStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stage, Meta = (AllowPrivateAccess = true))
	ALevelStreamerActor* NextPortal;

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ATODEnemy>> ArrEnemyType;

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ATODEnemy>> ArrBossType;

	UPROPERTY()
	TArray<ATODEnemySpawnPoint*> EnemySpawnPoint;

	UPROPERTY()
	TArray<ATODEnemy*> ArrEnemy;

	UPROPERTY()
	int32 EnemyDeadCount;

	UPROPERTY()
	ATODEnemy* BossEnemy;
};
