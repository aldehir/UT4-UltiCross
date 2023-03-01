#pragma once

#include "Core.h"
#include "SlateBasics.h"
#include "UltiCrosshairProxy.h"

class UUltiCrosshair;
class SUltiCrosshairViewModel;

typedef float (FUltiCrosshairProxy::*FloatGetter)() const;
typedef void (FUltiCrosshairProxy::*FloatSetter)(float);

class FSliderDelegate
{
public:
  FSliderDelegate(
    FUltiCrosshairProxy* Model,
    FloatGetter Getter,
    FloatSetter Setter,
    float Min,
    float Max,
    float Resolution
  ) : Model(Model)
    , Getter(Getter)
    , Setter(Setter)
    , Min(Min)
    , Max(Max)
    , Resolution(Resolution)
  {}

  void Set(float Value);
  float Get() const;
  FText Text() const;

  FUltiCrosshairProxy* Model;
  FloatGetter Getter;
  FloatSetter Setter;
  float Min;
  float Max;
  float Resolution;
};

/**
 * Crosshair Configuration ViewModel.
 */
class SUltiCrosshairViewModel
{
public:
  SUltiCrosshairViewModel(UUltiCrosshair *Crosshair);

  void SetCrosshair(UUltiCrosshair* Crosshair);
  inline UUltiCrosshair* GetCrosshair() const { return Proxy.GetCrosshair(); }

  FText GetCrosshairName() const;
  FSlateBrush* GetBrush() const { return Brush; }

  TSharedPtr<FSliderDelegate> ThicknessDelegate;
  TSharedPtr<FSliderDelegate> GapDelegate;
  TSharedPtr<FSliderDelegate> LengthDelegate;

private:
  FUltiCrosshairProxy Proxy;
  FSlateBrush* Brush;
};