#include "Enemy/TODEnemyGrux.h"
#include "Enemy/TODEnemyGruxAIController.h"

ATODEnemyGrux::ATODEnemyGrux()
{
	AIControllerClass = ATODEnemyGruxAIController::StaticClass();
	// ������ ��ġ�ϰų� ���Ӱ� �����Ǵ� Enemy���� ANNSEnemyAIController���� ���� �� ���踦 �޴´�.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	TODLOG_S(Warning);
}