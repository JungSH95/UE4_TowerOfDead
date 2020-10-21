#include "Enemy/TODEnemyMelee.h"
#include "Enemy/TODEnemyMeleeAIController.h"

ATODEnemyMelee::ATODEnemyMelee()
{
	AIControllerClass = ATODEnemyMeleeAIController::StaticClass();
	// 레벨에 배치하거나 새롭게 생성되는 Enemy마다 ANNSEnemyAIController액터 생성 및 지배를 받는다.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}