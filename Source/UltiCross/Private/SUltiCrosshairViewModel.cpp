#include "UltiCrossPCH.h"
#include "SUltiCrosshairViewModel.h"
#include "UltiCrosshair.h"
#include "FindProperty.h"

FConstrainedSliderDelegate::FConstrainedSliderDelegate(FUltiCrosshairViewModel* ViewModel, const FString& PropertyPath)
  : ViewModel(ViewModel)
  , PropertyPath(PropertyPath)
{
  check(ViewModel);
}

float FConstrainedSliderDelegate::Get() const
{
  UUltiCrosshair* Inst = ViewModel->GetCrosshair();
  TSharedRef<FUltiCrosshairConstraint> Constraint = Inst->GetConstraint(PropertyPath);
  return Constraint->Normalize(GetRaw());
}

float FConstrainedSliderDelegate::GetRaw() const
{
  UUltiCrosshair* Inst = ViewModel->GetCrosshair();

  void* Data = nullptr;
  UNumericProperty* Prop = FindPropertyChecked<UNumericProperty>(Inst, PropertyPath, &Data);

  if (Prop->IsInteger()) {
    return (float)Prop->GetSignedIntPropertyValue(Data);
  }

  return Prop->GetFloatingPointPropertyValue(Data);
}

void FConstrainedSliderDelegate::Set(float Value)
{
  UUltiCrosshair* Inst = ViewModel->GetCrosshair();
  TSharedRef<FUltiCrosshairConstraint> Constraint = Inst->GetConstraint(PropertyPath);

  Value = Constraint->Denormalize(Value);

  void* Data = nullptr;
  UNumericProperty* Prop = FindPropertyChecked<UNumericProperty>(Inst, PropertyPath, &Data);

  if (Prop->IsInteger()) {
    Prop->SetIntPropertyValue(Data, (int64)Value);
  } else {
    Prop->SetFloatingPointPropertyValue(Data, Value);
  }

  Inst->UpdateTexture();
}

FText FConstrainedSliderDelegate::Text() const
{
  return FText::AsNumber(GetRaw());
}

FUltiCrosshairViewModel::FUltiCrosshairViewModel()
  : Crosshair(nullptr)
  , CrosshairCDO(nullptr)
  , Brush(new FSlateBrush())
{
}

FUltiCrosshairViewModel::~FUltiCrosshairViewModel()
{
  delete Brush;
}

void FUltiCrosshairViewModel::SetCrosshair(UUltiCrosshair* C)
{
  check(C);
  Crosshair = C;
  CrosshairCDO = GetMutableDefault<UUltiCrosshair>(C->GetClass());
  Brush->SetResourceObject(C->GetTexture());
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

TSharedRef<FConstrainedSliderDelegate> FUltiCrosshairViewModel::GetDelegate(const FString& Path)
{
  TSharedRef<FConstrainedSliderDelegate>* Search = Delegates.Find(Path);
  if (Search != nullptr)
  {
    return *Search;
  }

  TSharedRef<FConstrainedSliderDelegate> Delegate = MakeShared<FConstrainedSliderDelegate>(this, Path);
  Delegates.Add(Path, Delegate);
  return Delegate;
}