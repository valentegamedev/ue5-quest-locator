#include "VLocatorComponent.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"

UVLocatorComponent::UVLocatorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVLocatorComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	
	UActorComponent* NewSplineComponent = Owner->AddComponentByClass(
		USplineComponent::StaticClass(),
		true,
		FTransform::Identity,
		false);
	OwnerSplineComponent = Cast<USplineComponent>(NewSplineComponent);

	UActorComponent* NewNiagaraComponent = Owner->AddComponentByClass(
		UNiagaraComponent::StaticClass(),
		true,
		FTransform::Identity,
		false);
	OwnerNiagaraComponent = Cast<UNiagaraComponent>(NewNiagaraComponent);
	OwnerNiagaraComponent->SetAsset(Particle);
}

void UVLocatorComponent::ShowPath(FVector Origin, FVector Target)
{
	//Clear any previous points in the spline (old path?)
	OwnerSplineComponent->ClearSplinePoints();

	//Request the path from Origin to Target from the Navigation System
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(GetOwner(), Origin, Target);

	//Iterate through all points and add them to the spline
	TArray<FNavPathPoint> PathPoints = Path->GetPath()->GetPathPoints();

	//Check if we have a path
	if (PathPoints.Num() > 0)
	{
		//Add the path points to the spline
		for (int32 Index = 0; Index != PathPoints.Num(); ++Index)
		{
			OwnerSplineComponent->AddSplinePoint(PathPoints[Index].Location,  ESplineCoordinateSpace::World);
			//Set the tangents to zero to avoid weird "knots"
			OwnerSplineComponent->SetTangentsAtSplinePoint(Index, FVector::ZeroVector, FVector::ZeroVector, ESplineCoordinateSpace::Local);
		}

		//Set the particle location to the beginning of the path
		OwnerNiagaraComponent->SetWorldLocation(PathPoints[0].Location);
		
		//Reset the simulation
		OwnerNiagaraComponent->ReinitializeSystem();
		//Reset to 0.0
		DistanceAlongSpline = 0.0f;
		
		//Clear any active timer
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		//Start the timer
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UVLocatorComponent::UpdatePath, 0.02f /* Repeat every 0.02 seconds */, true /* Loop */);
	}
}

void UVLocatorComponent::UpdatePath()
{
	//Get the timer rate
	float TimerRate = GetWorld()->GetTimerManager().GetTimerRate(TimerHandle);

	//Update the distance along the spline
	DistanceAlongSpline += Speed * TimerRate;

	//Check if it reached the end
	if (DistanceAlongSpline >= OwnerSplineComponent->GetSplineLength())
	{
		//Clear the timer
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	else
	{
		//Update particle location in world space
		FVector NewLocation = OwnerSplineComponent->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		OwnerNiagaraComponent->SetWorldLocation(NewLocation);

		//Update particle rotation in world space
		FRotator NewRotation = OwnerSplineComponent->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		OwnerNiagaraComponent->SetWorldRotation(NewRotation);
	}
}

UNiagaraSystem* UVLocatorComponent::GetParticle() const
{
	return Particle;
}

void UVLocatorComponent::SetParticle(UNiagaraSystem* NewParticle)
{
	Particle = NewParticle;
}

float UVLocatorComponent::GetSpeed() const
{
	return Speed;
}

void UVLocatorComponent::SetSpeed(float NewSpeed)
{
	Speed = NewSpeed;
}

