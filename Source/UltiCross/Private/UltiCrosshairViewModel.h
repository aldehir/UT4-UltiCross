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
  void SetRaw(float Value);

  FText Text() const;
  void CommitText(const FText& Input, ETextCommit::Type Commit);

private:
  FUltiCrosshairViewModel* ViewModel;
  FString PropertyPath;

  UUltiCrosshair* Obj;
  UNumericProperty* Prop;
  void* PropData;
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
  void CopyCrosshair(UUltiCrosshair* Other);

  FText GetName() const;
  void OnNameChanged(const FText& Name, ETextCommit::Type CommitType);

  FSlateBrush* GetBrush() const;

  FText GetTypeText() const;
  void OnTypeChanged(TSharedPtr<FUltiCrosshairTypeDescriptor> Descriptor, ESelectInfo::Type SelectType);
  TArray<TSharedPtr<FUltiCrosshairTypeDescriptor>>* GetCrosshairTypeDescriptors() { return &CrosshairTypes; }
  TSharedPtr<FUltiCrosshairTypeDescriptor> GetCrosshairTypeDescriptor() const;
  int32 GetCrosshairTypeDescriptorIndex() const;

  FLinearColor GetFillColor() const;
  void SetFillColor(FLinearColor Color);

  FLinearColor GetOutlineColor() const;
  void SetOutlineColor(FLinearColor Color);

  TSharedRef<FConstrainedSliderDelegate> GetDelegate(const FString& Path);

private:
  SUltiCrossConfigDialog* View;

  UUltiCrosshair* Crosshair;
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
