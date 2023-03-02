#pragma once

#include "Core.h"
#include "SlateBasics.h"

class UUltiCrosshair;
class FUltiCrosshairViewModel;

class FSliderDelegate
{
public:
  typedef float (FUltiCrosshairViewModel::*FloatGetter)() const;
  typedef void (FUltiCrosshairViewModel::*FloatSetter)(float);

  FSliderDelegate(
    FUltiCrosshairViewModel* Obj,
    FloatGetter Getter,
    FloatSetter Setter,
    float Min,
    float Max,
    float Resolution
  ) : Obj(Obj)
    , Getter(Getter)
    , Setter(Setter)
    , Min(Min)
    , Max(Max)
    , Resolution(Resolution)
  {}

  void Set(float Value);
  float Get() const;
  FText Text() const;

  FUltiCrosshairViewModel* Obj;
  FloatGetter Getter;
  FloatSetter Setter;
  float Min;
  float Max;
  float Resolution;
};

/**
 * Crosshair Configuration ViewModel.
 */
class FUltiCrosshairViewModel
{
public:
  FUltiCrosshairViewModel();
  ~FUltiCrosshairViewModel();

  void SetCrosshair(UUltiCrosshair* Crosshair);
  UUltiCrosshair* GetCrosshair() const;

  FText GetCrosshairName() const;
  FSlateBrush* GetBrush() const;

  EUltiCrossCrosshairType GetType() const;
  void SetType(EUltiCrossCrosshairType Type);

  float GetThickness() const;
  void SetThickness(float Value);

  float GetGap() const;
  void SetGap(float Value);

  float GetLength() const;
  void SetLength(float Value);

  TSharedPtr<FSliderDelegate> ThicknessDelegate; 
  TSharedPtr<FSliderDelegate> GapDelegate; 
  TSharedPtr<FSliderDelegate> LengthDelegate; 

private:
  UUltiCrosshair* Crosshair;
  UUltiCrosshair* CrosshairCDO;
  FSlateBrush* Brush;
};