#include "TODStageManager.h"
#include "Enemy/TODEnemy.h"
#include "Enemy/TODEnemyAIController.h"
#include "Enemy/TODEnemyStatComponent.h"
#include "TODEnemySpawnPoint.h"
#include "TODCharacter.h"
#include "TODGameMode.h"
#include "Components/WidgetComponent.h"

ATODStageManager::ATODStageManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_1(TEXT("/Game/BluePrint/Enemy/BP_Enemy_1.BP_Enemy_1_C"));
	if (ENEMYCLASS_1.Class != NULL)
		ArrEnemyType.Add(ENEMYCLASS_1.Class);

	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYCLASS_2(TEXT("/Game/BluePrint/Enemy/BP_Enemy_2.BP_Enemy_2_C"));
	if (ENEMYCLASS_2.Class != NULL)
		ArrEnemyType.Add(ENEMYCLASS_2.Class);

	ConstructorHelpers::FClassFinder<ATODEnemy> ENEMYBOSSCLASS_1(TEXT("/Game/BluePrint/Enemy/BP_Boss1.BP_Boss1_C"));
	if (ENEMYBOSSCLASS_1.Class != NULL)
		ArrBossType.Add(ENEMYBOSSCLASS_1.Class);
	
	IsClear = false;
	IsBattleStart = false;
	EnemyDeadCount = 0;
}

void ATODStageManager::BeginPlay()
{
	Super::BeginPlay();

	ATODGameMode* gameMode = Cast<ATODGameMode>(GetWorld()->GetAuthGameMode());
	if (gameMode != nullptr)
	{
		TODGameMode = gameMode;
		TODGameMode->AddArrStageManager(this);
	}

	// StartPoint & Portal & EnemySawnPoint ã��
	if (StartPoint == nullptr || NextPortal == nullptr)
	{
		TArray<AActor*>  OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), OutActors);
		for (int i = 0; i < OutActors.Num(); i++)
		{
			// �ش� Stage�� �ִ� ��
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
		// FadeOut ���� ���� �ƴ϶��
		if (!TODGameMode->GetIsSequencePlaying())
		{
			print(FString::Printf(TEXT("Battle Start")));

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

	if (TODGameMode != nullptr)
	{
		// 3��° ������ ȸ�� or ����
		if ((TODGameMode->GetStageCount() == 2) && (!TODGameMode->GetIsSafeZone()))
		{
			FString SNextStageName = FString::Printf(TEXT("SafeStageMap"));
			NextPortal->SetNextLevel(FName(*SNextStageName));
		}
		// 4��° ������ ���� ��������
		else if (TODGameMode->GetStageCount() == 2)
		{
			FString SNextStageName = FString::Printf(TEXT("BossStageMap"));
			NextPortal->SetNextLevel(FName(*SNextStageName));
		}
		else
		{
			// ���� ���������� ����? ����?
			int RandomNumber = FMath::RandRange(1, 2);
			FString SNextStageName = FString::Printf(TEXT("StageMap%d"), RandomNumber);
			NextPortal->SetNextLevel(FName(*SNextStageName));
		}

		// ���� ���� �̵��� �� �ְ� �浹 �̺�Ʈ Ȱ��ȭ
		NextPortal->SetNextLevelEvent(true);
		NextPortal->SetPortalEffectActive(true);

		IsClear = false;
	}
}

void ATODStageManager::SetPlayerPosition()
{
	// �÷��̾� ĳ���� ã�Ƽ� �ش� ��ġ�� �̵�
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
	BossEnemy = nullptr;

	for (int i = 0; i < EnemySpawnPoint.Num(); i++)
	{
		// �Ϲ� ���� ��ȯ
		if (!EnemySpawnPoint[i]->GetIsBoss())
		{
			if (EnemySpawnPoint[i]->GetEnemyNumber() > 0 && EnemySpawnPoint[i]->GetEnemyNumber() <= ArrEnemyType.Num())
			{
				ATODEnemy* Enemy = GetWorld()->SpawnActor<ATODEnemy>(ArrEnemyType[EnemySpawnPoint[i]->GetEnemyNumber() - 1],
					EnemySpawnPoint[i]->GetActorLocation(), EnemySpawnPoint[i]->GetActorRotation());
				if (Enemy != nullptr)
				{
					Enemy->EnemyStat->SetNewLevel(EnemySpawnPoint[i]->GetEnemyLevel());
					Enemy->OnEnemyDeadCheck.AddUObject(this, &ATODStageManager::StageClearCheck);
					ArrEnemy.Add(Enemy);
				}
			}
		}
		// �������� ��ȯ
		else
		{
			if (EnemySpawnPoint[i]->GetEnemyNumber() > 0 && 
				EnemySpawnPoint[i]->GetEnemyNumber() <= ArrBossType.Num())
			{
				print(FString::Printf(TEXT("Boss Enemy Init")));

				BossEnemy = GetWorld()->SpawnActor<ATODEnemy>(ArrBossType[EnemySpawnPoint[i]->GetEnemyNumber() - 1],
					EnemySpawnPoint[i]->GetActorLocation(), EnemySpawnPoint[i]->GetActorRotation());
				if (BossEnemy != nullptr)
				{
					BossEnemy->EnemyStat->SetNewLevel(EnemySpawnPoint[i]->GetEnemyLevel());
					BossEnemy->OnEnemyDeadCheck.AddUObject(this, &ATODStageManager::StageClearCheck);
				}
			}
		}
	}

	print(FString::Printf(TEXT("Enemy count : %d"), ArrEnemy.Num()));
}

void ATODStageManager::StageStart()
{
	if (TODGameMode == nullptr)
		return;

	TODGameMode->PlayFadeIn();

	// �Ϲ� ���� AI ���� & HP UI Ȱ��ȭ
	for (int i = 0; i < ArrEnemy.Num(); i++)
	{
		ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(ArrEnemy[i]->GetController());
		if (EnemyAI != nullptr)
			EnemyAI->StartAI();
		ArrEnemy[i]->HPBarWidget->SetHiddenInGame(false);
	}

	// ���� ���� AI ���� & HP UI Ȱ��ȭ
	if(BossEnemy != nullptr)
	{
		ATODEnemyAIController* EnemyAI = Cast<ATODEnemyAIController>(BossEnemy->GetController());
		if (EnemyAI != nullptr)
			EnemyAI->StartAI();
		
		// BOSS ���� HP UI Ȱ��ȭ
		ATODCharacter* playerCharacter = Cast<ATODCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (playerCharacter != nullptr)
		{
			print(FString::Printf(TEXT("Boss Stat Bind")));
			playerCharacter->SetBossEnemyStatBind(BossEnemy->EnemyStat);
		}

		BossEnemy->LevelStartMontage();
	}

	if (ArrEnemy.Num() == 0 && BossEnemy == nullptr)
		IsClear = true;
}

void ATODStageManager::StageClearCheck()
{
	EnemyDeadCount++;

	// Boss üũ�� ��� �� ���ΰ�?

	if (EnemyDeadCount >= ArrEnemy.Num())
	{
		IsClear = true;
		print(FString::Printf(TEXT("Stage Clear")));
	}
}

void ATODStageManager::ReSetStage()
{
	// �ʱ����
	IsBattleStart = false;
	EnemyDeadCount = 0;

	TODLOG_S(Warning);
}