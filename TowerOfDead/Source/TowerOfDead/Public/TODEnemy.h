#pragma once

#include "CoreMinimal.h"
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

};
