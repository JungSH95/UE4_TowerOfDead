#pragma once

#include "TowerOfDead.h"
#include "GameFramework/Character.h"
#include "TODEnemy.generated.h"

UCLASS()
class TOWEROFDEAD_API ATODEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ATODEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	EnemyState GetState() { return State; }
	void SetState(EnemyState newState) { State = newState; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	EnemyState State;
};
