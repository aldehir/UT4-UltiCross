#pragma once

#include "Core.h"
#include "SlateBasics.h"
#include "UltiCrosshairConstraints.h"

class UUltiCrosshair;
class FUltiCrosshairViewModel;

class FConstrainedSliderDelegate
{
public:
  FConstrainedSliderDelegate(FUltiCrosshairViewModel* ViewModel, const FString& PropertyPath);

  float Get() const;
  float GetRaw() const;

  void Set(float value);
  FText Text() const;

private:
  FUltiCrosshairViewModel* ViewModel;
  FString PropertyPath;
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

  TSharedRef<FConstrainedSliderDelegate> GetDelegate(const FString& Path);

private:
  UUltiCrosshair* Crosshair;
  UUltiCrosshair* CrosshairCDO;
  FSlateBrush* Brush;

  TMap<FString, TSharedRef<FConstrainedSliderDelegate>> Delegates;
};