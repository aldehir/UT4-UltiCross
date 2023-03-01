#include "UltiCrossPCH.h"
#include "SUltiCrosshairViewModel.h"
#include "UltiCrosshair.h"

static float NormalizeValue(float Value, float Min, float Max)
{
  return (Value - Min) / (Max - Min);
}

static float ScaleValue(float Value, float Min, float Max)
{
  return Min + ((Max - Min) * Value);
}

SUltiCrosshairViewModel::SUltiCrosshairViewModel(UUltiCrosshair *Crosshair) : Model(Crosshair)
{
  Brush = new FSlateBrush();
}

void SUltiCrosshairViewModel::SetModel(UUltiCrosshair* Crosshair) {
  Model = Crosshair;
  Brush->SetResourceObject(Model->GetTexture());
}

FText SUltiCrosshairViewModel::GetCrosshairName() const
{
  if (Model == nullptr)
  {
    return FText();
  }

  return Model->CrosshairName;
}

#define HANDLE_SLIDER_CHANGE(Attr, Value, Min, Max) \
  do { \
    if (Model == nullptr) return; \
    float NewValue = FMath::RoundToInt(ScaleValue((Value), (Min), (Max))); \
    if (Model->Attr != NewValue) \
    { \
      Model->Attr = NewValue; \
      Model->UpdateTexture(); \
    } \
  } while (0)

#define HANDLE_SLIDER_GET_VALUE(Attr, Min, Max) \
  do { \
    if (Model == nullptr) return 0.0f; \
    return NormalizeValue(Model->Attr, (Min), (Max)); \
  } while (0)

#define HANDLE_SLIDER_GET_VALUE_AS_TEXT(Attr) \
  do { \
    if (Model == nullptr) return FText(); \
    return FText::AsNumber(Model->Attr); \
  } while (0)

void SUltiCrosshairViewModel::OnThicknessSliderChange(float Value)
{
  HANDLE_SLIDER_CHANGE(Thickness, Value, ULTICROSS_THICKNESS_MIN, ULTICROSS_THICKNESS_MAX);
}

float SUltiCrosshairViewModel::GetThicknessForSlider() const
{
  HANDLE_SLIDER_GET_VALUE(Thickness, ULTICROSS_THICKNESS_MIN, ULTICROSS_THICKNESS_MAX);
}

FText SUltiCrosshairViewModel::GetThicknessAsText() const
{
  HANDLE_SLIDER_GET_VALUE_AS_TEXT(Thickness);
}

void SUltiCrosshairViewModel::OnGapSliderChange(float Value)
{
  HANDLE_SLIDER_CHANGE(Gap, Value, 0.0f, 10.0f);
}

float SUltiCrosshairViewModel::GetGapForSlider() const
{
  HANDLE_SLIDER_GET_VALUE(Gap, 0.0f, 10.0f);
}

FText SUltiCrosshairViewModel::GetGapAsText() const
{
  HANDLE_SLIDER_GET_VALUE_AS_TEXT(Gap);
}

void SUltiCrosshairViewModel::OnLengthSliderChange(float Value)
{
  HANDLE_SLIDER_CHANGE(Length, Value, 0.0f, 30.0f);
}

float SUltiCrosshairViewModel::GetLengthForSlider() const
{
  HANDLE_SLIDER_GET_VALUE(Length, 0.0f, 30.0f);
}

FText SUltiCrosshairViewModel::GetLengthAsText() const
{
  HANDLE_SLIDER_GET_VALUE_AS_TEXT(Length);
}