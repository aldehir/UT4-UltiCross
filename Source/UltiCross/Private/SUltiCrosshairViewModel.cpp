#include "UltiCrossPCH.h"
#include "SUltiCrosshairViewModel.h"
#include "UltiCrosshair.h"

void FSliderDelegate::Set(float Value)
{
  Value = Min + ((Max - Min) * Value);
  Value = FMath::FloorToInt(Value * (1.0f / Resolution)) * Resolution;

  (Model->*Setter)(Value);
}

float FSliderDelegate::Get() const
{
  float Value = (Model->*Getter)();
  return (Value - Min) / (Max - Min);
}

FText FSliderDelegate::Text() const
{
  float Value = (Model->*Getter)();
  return FText::AsNumber(Value);
}

#define ASSIGN_SLIDER_DELEGATE(Attr) \
  do { \
    Attr##Delegate = MakeShareable( \
      new FSliderDelegate( \
        &Proxy, \
        &FUltiCrosshairProxy::Get##Attr, \
        &FUltiCrosshairProxy::Set##Attr, \
        UltiCrosshairConstraint::Attr##Min, \
        UltiCrosshairConstraint::Attr##Max, \
        UltiCrosshairConstraint::Attr##Resolution \
      ) \
    ); \
  } while (0)

SUltiCrosshairViewModel::SUltiCrosshairViewModel(UUltiCrosshair *Crosshair)
{
  Proxy.SetCrosshair(Crosshair);
  Brush = new FSlateBrush();

  ASSIGN_SLIDER_DELEGATE(Thickness);
  ASSIGN_SLIDER_DELEGATE(Gap);
  ASSIGN_SLIDER_DELEGATE(Length);
}

void SUltiCrosshairViewModel::SetCrosshair(UUltiCrosshair* Crosshair) {
  Proxy.SetCrosshair(Crosshair);
  Brush->SetResourceObject(Crosshair->GetTexture());
}

FText SUltiCrosshairViewModel::GetCrosshairName() const
{
  return Proxy.GetCrosshairName();
}