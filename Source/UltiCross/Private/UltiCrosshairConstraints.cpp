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

#define CONSTRAINTS_START(Path) \
  do { \
    TSharedRef<FUltiCrosshairConstraintMap> M = MakeShared<FUltiCrosshairConstraintMap>(); \
    Constraints.Add(TEXT(Path), M)

#define CONSTRAINT_DEFAULT(Type, ...) M->Default = MakeShared<Type>(__VA_ARGS__)
#define CONSTRAINT_ADD(CType, Type, ...) M->Add(EUltiCrossCrosshairType::##CType, MakeShared<Type>(__VA_ARGS__))
#define CONSTRAINTS_END() } while (0)

FUltiCrosshairConstraints::FUltiCrosshairConstraints()
{
  CONSTRAINTS_START("Thickness");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 1.0f, 20.0f, 1.0f);
  CONSTRAINT_ADD(Crosshairs, FUltiCrosshairThicknessConstraint, 20.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Rotation");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 360.0f, 45.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Outline");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 10.0f, 1.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("DotRadius");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 40.0f, 1.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Crosshairs.Count");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 2.0f, 8.0f, 1.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Crosshairs.CenterGap");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 40.0f, 1.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Crosshairs.Length");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 40.0f, 1.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Circle.Radius");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 30.0f, 1.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Ngon.Edges");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 3.0f, 8.0f, 1.0f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Ngon.RadiusX");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 60.0f, 0.5f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Ngon.RadiusY");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 60.0f, 0.5f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Ngon.OffsetX");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 5.0f, 0.5f);
  CONSTRAINTS_END();

  CONSTRAINTS_START("Ngon.OffsetY");
  CONSTRAINT_DEFAULT(FUltiCrosshairDiscreteConstraint, 0.0f, 5.0f, 0.5f);
  CONSTRAINTS_END();
}
