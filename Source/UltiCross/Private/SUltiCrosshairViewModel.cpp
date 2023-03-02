#include "UltiCrossPCH.h"
#include "SUltiCrosshairViewModel.h"
#include "UltiCrosshair.h"

void FSliderDelegate::Set(float Value)
{
  Value = Min + ((Max - Min) * Value);
  Value = FMath::FloorToInt(Value * (1.0f / Resolution)) * Resolution;

  (Obj->*Setter)(Value);
}

float FSliderDelegate::Get() const
{
  float Value = (Obj->*Getter)();
  return (Value - Min) / (Max - Min);
}

FText FSliderDelegate::Text() const
{
  float Value = (Obj->*Getter)();
  return FText::AsNumber(Value);
}

#define ASSIGN_SLIDER_DELEGATE(Attr) \
  do { \
    Attr##Delegate = MakeShareable( \
      new FSliderDelegate( \
        &this, \
        &FUltiCrosshairViewModel::Get##Attr, \
        &FUltiCrosshairViewModel::Set##Attr, \
        UltiCrosshairConstraint::Attr##Min, \
        UltiCrosshairConstraint::Attr##Max, \
        UltiCrosshairConstraint::Attr##Resolution \
      ) \
    ); \
  } while (0)

FUltiCrosshairViewModel::FUltiCrosshairViewModel()
  : Crosshair(nullptr)
  , CrosshairCDO(nullptr)
  , Brush(new FSlateBrush())
{
  ThicknessDelegate = MakeShared<FSliderDelegate>(
    this, &FUltiCrosshairViewModel::GetThickness, &FUltiCrosshairViewModel::SetThickness,
    UltiCrosshairConstraint::ThicknessMin, UltiCrosshairConstraint::ThicknessMax,
    UltiCrosshairConstraint::ThicknessResolution
  );

  GapDelegate = MakeShared<FSliderDelegate>(
    this, &FUltiCrosshairViewModel::GetGap, &FUltiCrosshairViewModel::SetGap,
    UltiCrosshairConstraint::GapMin, UltiCrosshairConstraint::GapMax,
    UltiCrosshairConstraint::GapResolution
  );

  LengthDelegate = MakeShared<FSliderDelegate>(
    this, &FUltiCrosshairViewModel::GetLength, &FUltiCrosshairViewModel::SetLength,
    UltiCrosshairConstraint::LengthMin, UltiCrosshairConstraint::LengthMax,
    UltiCrosshairConstraint::LengthResolution
  );
}

FUltiCrosshairViewModel::~FUltiCrosshairViewModel()
{
  delete Brush;
}

void FUltiCrosshairViewModel::SetCrosshair(UUltiCrosshair* C)
{
  Crosshair = C;
  if (C != nullptr)
  {
    CrosshairCDO = GetMutableDefault<UUltiCrosshair>(C->GetClass());
    Brush->SetResourceObject(C->GetTexture());
  }
}

UUltiCrosshair* FUltiCrosshairViewModel::GetCrosshair() const
{
  return Crosshair;
}

FText FUltiCrosshairViewModel::GetCrosshairName() const
{
  return Crosshair->CrosshairName;
}

FSlateBrush* FUltiCrosshairViewModel::GetBrush() const
{
  return Brush;
}

EUltiCrossCrosshairType FUltiCrosshairViewModel::GetType() const
{
  return Crosshair->Type;
}

void FUltiCrosshairViewModel::SetType(EUltiCrossCrosshairType Type)
{
  Crosshair->Type = CrosshairCDO->Type = Type;
  Crosshair->UpdateTexture();
}

float FUltiCrosshairViewModel::GetThickness() const
{
  return Crosshair->Thickness;
}

void FUltiCrosshairViewModel::SetThickness(float Value)
{
  Crosshair->Thickness = CrosshairCDO->Thickness = Value;
  Crosshair->UpdateTexture();
}

float FUltiCrosshairViewModel::GetGap() const
{
  return Crosshair->Gap;
}

void FUltiCrosshairViewModel::SetGap(float Value)
{
  Crosshair->Gap = CrosshairCDO->Gap = Value;
  Crosshair->UpdateTexture();
}

float FUltiCrosshairViewModel::GetLength() const
{
  return Crosshair->Length;
}

void FUltiCrosshairViewModel::SetLength(float Value)
{
  Crosshair->Length = CrosshairCDO->Length = Value;
  Crosshair->UpdateTexture();
}