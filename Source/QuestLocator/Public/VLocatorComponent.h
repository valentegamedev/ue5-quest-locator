#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VLocatorComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUESTLOCATOR_API UVLocatorComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	class USplineComponent* OwnerSplineComponent;

	UPROPERTY()
	class UNiagaraComponent* OwnerNiagaraComponent;

	UPROPERTY(EditAnywhere, BlueprintGetter=GetParticle, BlueprintSetter=SetParticle, Category = "Locator")
	class UNiagaraSystem* Particle;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintGetter=GetSpeed, BlueprintSetter=SetSpeed, EditAnywhere, Category="Locator")
	float Speed;

	UPROPERTY()
	float DistanceAlongSpline;
	
	UFUNCTION()
	void UpdatePath();
public:	
	UVLocatorComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void ShowPath(FVector Origin, FVector Target);

	UFUNCTION(BlueprintGetter)
	UNiagaraSystem* GetParticle() const;

	UFUNCTION(BlueprintSetter)
	void SetParticle(UNiagaraSystem* NewParticle);
	
	UFUNCTION(BlueprintGetter)
	float GetSpeed() const;

	UFUNCTION(BlueprintSetter)
	void SetSpeed(float NewSpeed);
};
