#pragma once

#include "Core.h"
#include "SlateBasics.h"

class UUltiCrosshair;

/**
 * Crosshair Configuration ViewModel.
 */
class SUltiCrosshairViewModel
{
public:
  SUltiCrosshairViewModel(UUltiCrosshair *Crosshair);

  void SetModel(UUltiCrosshair* Crosshair);
  inline UUltiCrosshair* GetModel() const { return Model; }

  FText GetCrosshairName() const;

  void OnThicknessSliderChange(float Thickness);
  float GetThicknessForSlider() const;
  FText GetThicknessAsText() const;

  void OnGapSliderChange(float Gap);
  float GetGapForSlider() const;
  FText GetGapAsText() const;

  void OnLengthSliderChange(float Length);
  float GetLengthForSlider() const;
  FText GetLengthAsText() const;

  FSlateBrush* GetBrush() const { return Brush; }

private:
  UUltiCrosshair* Model;
  FSlateBrush* Brush;
};