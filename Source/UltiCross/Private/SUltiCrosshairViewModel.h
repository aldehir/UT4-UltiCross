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

  void CacheReferences();

  float Get() const;
  float GetRaw() const;

  void Set(float value);
  FText Text() const;

private:
  struct CachedRef
  {
    UUltiCrosshair* Obj;
    UNumericProperty* Prop;
    void* PropData;
  };

  FUltiCrosshairViewModel* ViewModel;
  FString PropertyPath;

  CachedRef CachedInstance;
  CachedRef CachedCDO;
  TSharedRef<FUltiCrosshairConstraint> CachedConstraint;
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