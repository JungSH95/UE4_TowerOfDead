#include "TODStageManager.h"
#include "TODEnemy.h"
#include "TODEnemyAIController.h"
#include "TODEnemyStatComponent.h"
#include "TODEnemySpawnPoint.h"
#include "TODCharacter.h"
#include "TODGameMode.h"
#include "Components/WidgetComponent.h"

ATODStageManager::ATODStageManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_1(TEXT("/Game/BluePrint/Enemy/BP_TESTEnemy_1.BP_TESTEnemy_1_C"));
	if (ENEMYCLASS_1.Class != NULL)
		ArrEnemyType.Add(ENEMYCLASS_1.Class);

	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_2(TEXT("/Game/BluePrint/Enemy/BP_TESTEnemy_2.BP_TESTEnemy_2_C"));
	if (ENEMYCLASS_2.Class != NULL)
		ArrEnemyType.Add(ENEMYCLASS_2.Class);

	IsClear = false;
	IsBattleStart = false;
	EnemyDeadCount = 0;
}

void ATODStageManager::BeginPlay()
{
	Super::BeginPlay();

	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
		TODGameMode = gameMode;
	TODGameMode->AddArrStageManager(this);

	// StartPoint & Portal & EnemySawnPoint 찾기
	if (StartPoint == nullptr || NextPortal == nullptr)
	{
		TArray<AActor*>  OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);
		for (int i = 0; i < OutActors.Num(); i++)
		{
			// 해당 Stage에 있는 것
			if (OutActors[i]->ActorHasTag(LevelName))
			{
				if (OutActors[i]->ActorHasTag("Portal"))
				{
					NextPortal = Cast<ALevelStreamerActor>(OutActors[i]);
					NextPortal->SetNowLevel(this, LevelName);
				}
				else if (OutActors[i]->ActorHasTag("StartPoint"))
					StartPoint = OutActors[i];
				else if (OutActors[i]->ActorHasTag("EnemySpawnPoint"))
					EnemySpawnPoint.Add(Cast<ATODEnemySpawnPoint>(OutActors[i]));
			}
		}
	}
}

void ATODStageManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TODGameMode != nullptr && !IsBattleStart)
	{
		// FadeOut 진행 중이 아니라면
		if (!TODGameMode->GetIsSequencePlaying())
		{
			IsBattleStart = true;
			SetPlayerPosition();
			
			NextPortal->SetPortalEffectActive(false);
			InitEnemy();

			GetWorldTimerManager().SetTimer(StartTimerHandle, this,
				&ATODStageManager::StageStart, 1.0f, false);
		}
	}

	if (IsClear)
		SetNextStage();
}

void ATODStageManager::SetNextStage()
{
	if (NextPortal == nullptr)
		return;

	// 다음 스테이지를 랜덤? 순차?
	int RandomNumber = FMath::RandRange(1, 2);
	FString SNextStageName = FString::Printf(TEXT("StageMap%d"), RandomNumber);
	NextPortal->SetNextLevel(FName(*SNextStageName));

	// 다음 지역 이동할 수 있게 충돌 이벤트 활성화
	NextPortal->SetNextLevelEvent(true);
	NextPortal->SetPortalEffectActive(true);

	IsClear = false;
}

void ATODStageManager::SetPlayerPosition()
{
	// 플레이어 캐릭터 찾아서 해당 위치로 이동
	ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (playerCharacter != nullptr && StartPoint != nullptr)
	{
		playerCharacter->SetActorLocation(StartPoint->GetActorLocation());
		playerCharacter->GetController()->SetControlRotation(StartPoint->GetActorRotation());
	}
}

void ATODStageManager::InitEnemy()
{
	ArrEnemy.Empty();

	for (int i = 0; i < EnemySpawnPoint.Num(); i++)
	{
		if (EnemySpawnPoint[i]->GetEnemyNumber() > 0 && EnemySpawnPoint[i]->GetEnemyNumber() <= ArrEnemyType.Num())
		{
			ATODEnemy* Enemy = GetWorld()->SpawnActor<ATODEnemy>(ArrEnemyType[EnemySpawnPoint[i]->GetEnemyNumber() - 1],
				EnemySpawnPoint[i]->GetActorLocation(), EnemySpawnPoint[i]->GetActorRotation());
			ArrEnemy.Add(Enemy);

			Enemy->EnemyStat->SetNewLevel(EnemySpawnPoint[i]->GetEnemyLevel());
			Enemy->OnEnemyDeadCheck.AddUObject(this, &ATODStageManager::StageClearCheck);
		}
	}

	print(FString::Printf(TEXT("Enemy count : %d"), ArrEnemy.Num()));
}

void ATODStageManager::StageStart()
{
	TODGameMode->PlayFadeIn();

	for (int i = 0; i < ArrEnemy.Num(); i++)
	{
		ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(ArrEnemy[i]->GetController());
		if (EnemyAI != nullptr)
			EnemyAI->StartAI();
		ArrEnemy[i]->HPBarWidget->SetHiddenInGame(false);
	}

	if (ArrEnemy.Num() == 0)
		IsClear = true;
}

void ATODStageManager::StageClearCheck()
{
	EnemyDeadCount++;

	if (EnemyDeadCount >= ArrEnemy.Num())
	{
		IsClear = true;
		print(FString::Printf(TEXT("Stage Clear")));
	}
}

void ATODStageManager::ReSetStage()
{
	// 초기상태
	IsBattleStart = false;
	EnemyDeadCount = 0;

	TODLOG_S(Warning);
}