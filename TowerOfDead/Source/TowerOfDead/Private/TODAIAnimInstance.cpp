#include "TODAIAnimInstance.h"
#include "TODEnemy.h"

UTODAIAnimInstance::UTODAIAnimInstance()
{
	CurrentSpeed = 0.0f;
	CurrentPawnDirection = 0.0f;
	IsDead = false;

	State = EnemyState::PEACE;
}

void UTODAIAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto EnemyPawn = TryGetPawnOwner();
	if (::IsValid(EnemyPawn))
	{
		CurrentSpeed = EnemyPawn->GetVelocity().Size();
		
		ATODEnemy* EnemyClass = Cast<ATODEnemy>(EnemyPawn);
		if (EnemyClass != nullptr)
		{
			State = EnemyClass->GetState();
			IsDead = EnemyClass->GetIsDead();
		}
	}
}