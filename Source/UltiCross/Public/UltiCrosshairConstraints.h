#pragma once

#include "Core.h"
#include "UltiCrosshairTypes.h"

class ULTICROSS_API FUltiCrosshairConstraint
{
public:
  FUltiCrosshairConstraint();
  FUltiCrosshairConstraint(float Min, float Max);

  virtual float Clamp(float V) const;
  virtual float Round(float V) const;

  virtual float Normalize(float V) const;
  virtual float Denormalize(float V) const;

  float Min;
  float Max;
};

class ULTICROSS_API FUltiCrosshairDiscreteConstraint : public FUltiCrosshairConstraint
{
public:
  FUltiCrosshairDiscreteConstraint(float Min, float Max, float Precision);

  virtual float Round(float V) const override;

  float Precision;
};

class ULTICROSS_API FUltiCrosshairThicknessConstraint : public FUltiCrosshairDiscreteConstraint
{
public:
  FUltiCrosshairThicknessConstraint(float Max);

  float Round(float V) const override;
};

class ULTICROSS_API FUltiCrosshairConstraintMap
{
public:
  FUltiCrosshairConstraintMap();
  FUltiCrosshairConstraintMap(TSharedRef<FUltiCrosshairConstraint> Default);

  float Clamp(EUltiCrossCrosshairType Type, float V) const;
  float Round(EUltiCrossCrosshairType Type, float V) const;

  void Add(EUltiCrossCrosshairType Type, TSharedRef<FUltiCrosshairConstraint> Constraint);
  TSharedRef<FUltiCrosshairConstraint> Get(EUltiCrossCrosshairType);

  TSharedRef<FUltiCrosshairConstraint> Default;
  TMap<EUltiCrossCrosshairType, TSharedRef<FUltiCrosshairConstraint>> Constraints;
};

class ULTICROSS_API FUltiCrosshairConstraints
{
public:
  FUltiCrosshairConstraints();

  TSharedRef<FUltiCrosshairConstraintMap> FindChecked(const FString& Path)
  {
    return Constraints.FindChecked(Path);
  }

  TMap<FString, TSharedRef<FUltiCrosshairConstraintMap>> Constraints;
};
