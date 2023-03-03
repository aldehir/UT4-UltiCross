#include "UltiCrossPCH.h"
#include "UltiCrosshairConstraints.h"

static float RoundToPrecision(float V, float Precision)
{
  return FMath::FloorToInt(V * (1.0f / Precision)) * Precision;
}

FUltiCrosshairConstraint::FUltiCrosshairConstraint() : Min(0.0f), Max(100.0f) {}
FUltiCrosshairConstraint::FUltiCrosshairConstraint(float Min, float Max) : Min(Min), Max(Max) {}

float FUltiCrosshairConstraint::Round(float V) const
{
  return V;
}

float FUltiCrosshairConstraint::Clamp(float V) const
{
  return V;
}

float FUltiCrosshairConstraint::Normalize(float V) const
{
  return (Round(Clamp(V)) - Min) / (Max - Min);
}

float FUltiCrosshairConstraint::Denormalize(float V) const
{
  return Round(Min + (FMath::Clamp(V, 0.0f, 1.0f) * (Max - Min)));
}

FUltiCrosshairDiscreteConstraint::FUltiCrosshairDiscreteConstraint(float Min, float Max, float Precision)
  : FUltiCrosshairConstraint(Min, Max), Precision(Precision)
{}

float FUltiCrosshairDiscreteConstraint::Round(float V) const
{
  return RoundToPrecision(V, Precision);
}

FUltiCrosshairThicknessConstraint::FUltiCrosshairThicknessConstraint(float Max)
  : FUltiCrosshairDiscreteConstraint(0.5f, Max, 1.0f)
{}

float FUltiCrosshairThicknessConstraint::Round(float V) const
{
  return V < 1.0f ? RoundToPrecision(V, 0.5f) : FUltiCrosshairDiscreteConstraint::Round(V);
}

FUltiCrosshairConstraintMap::FUltiCrosshairConstraintMap() : Default(new FUltiCrosshairConstraint) {}
FUltiCrosshairConstraintMap::FUltiCrosshairConstraintMap(TSharedRef<FUltiCrosshairConstraint> Default) : Default(Default) {}

TSharedRef<FUltiCrosshairConstraint> FUltiCrosshairConstraintMap::Get(EUltiCrossCrosshairType Type)
{
  TSharedRef<FUltiCrosshairConstraint>* Constraint = Constraints.Find(Type);
  if (Constraint != nullptr)
  {
    return *Constraint;
  }

  return Default;
}

void FUltiCrosshairConstraintMap::Add(EUltiCrossCrosshairType Type, TSharedRef<FUltiCrosshairConstraint> Constraint)
{
  Constraints.Add(Type, Constraint);
}

FUltiCrosshairConstraints::FUltiCrosshairConstraints()
{
  // Thickness Constraints
  TSharedRef<FUltiCrosshairConstraintMap> ThicknessConstraints = MakeShared<FUltiCrosshairConstraintMap>();
  ThicknessConstraints->Default = MakeShared<FUltiCrosshairDiscreteConstraint>(1.0, 20.0f, 1.0f);
  ThicknessConstraints->Add(EUltiCrossCrosshairType::Crosshairs, MakeShared<FUltiCrosshairThicknessConstraint>(20.0f));

  Constraints.Add(TEXT("Thickness"), ThicknessConstraints);

  // Gap Constraints
  TSharedRef<FUltiCrosshairConstraintMap> GapConstraints = MakeShared<FUltiCrosshairConstraintMap>();
  GapConstraints->Default = MakeShared<FUltiCrosshairDiscreteConstraint>(0.0, 20.0f, 1.0f);

  Constraints.Add(TEXT("Gap"), GapConstraints);

  // Length Constraints
  TSharedRef<FUltiCrosshairConstraintMap> LengthConstraints = MakeShared<FUltiCrosshairConstraintMap>();
  LengthConstraints->Default = MakeShared<FUltiCrosshairDiscreteConstraint>(0.0, 40.0f, 1.0f);

  Constraints.Add(TEXT("Length"), LengthConstraints);
}