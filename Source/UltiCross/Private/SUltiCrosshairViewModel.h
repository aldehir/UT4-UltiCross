#pragma once

#include "Core.h"
#include "SlateBasics.h"
#include "UltiCrosshairConstraints.h"

class UUltiCrosshair;
class FUltiCrosshairViewModel;
struct FUltiCrosshairTypeDescriptor;

class SUltiCrossConfigDialog;

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
  FUltiCrosshairViewModel(SUltiCrossConfigDialog* View);
  ~FUltiCrosshairViewModel();

  void SetCrosshair(UUltiCrosshair* Crosshair);
  UUltiCrosshair* GetCrosshair() const;

  FText GetCrosshairName() const;
  FSlateBrush* GetBrush() const;

  FText GetTypeText() const;
  void OnTypeChanged(TSharedPtr<FUltiCrosshairTypeDescriptor> Descriptor, ESelectInfo::Type SelectType);
  TArray<TSharedPtr<FUltiCrosshairTypeDescriptor>>* GetCrosshairTypeDescriptors() { return &CrosshairTypes; }
  TSharedPtr<FUltiCrosshairTypeDescriptor> GetCrosshairTypeDescriptor() const;
  int32 GetCrosshairTypeDescriptorIndex() const;

  TSharedRef<FConstrainedSliderDelegate> GetDelegate(const FString& Path);

private:
  SUltiCrossConfigDialog* View;

  UUltiCrosshair* Crosshair;
  UUltiCrosshair* CrosshairCDO;
  FSlateBrush* Brush;

  TArray<TSharedPtr<FUltiCrosshairTypeDescriptor>> CrosshairTypes;
  TMap<FString, TSharedRef<FConstrainedSliderDelegate>> Delegates;
};

struct FUltiCrosshairTypeDescriptor
{
  FUltiCrosshairTypeDescriptor(EUltiCrossCrosshairType Type, const FString& Description)
    : Type(Type), Description(Description)
  {}

  EUltiCrossCrosshairType Type;
  FString Description;

  FText Text() const {
    return FText::FromString(Description);
  }
};

TSharedPtr<FUltiCrosshairTypeDescriptor> MakeTypeDescriptor(EUltiCrossCrosshairType Type, const FString& Description)
{
  return MakeShareable(new FUltiCrosshairTypeDescriptor(Type, Description));
}
