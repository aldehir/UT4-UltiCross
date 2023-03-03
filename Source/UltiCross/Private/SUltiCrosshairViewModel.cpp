#include "UltiCrossPCH.h"
#include "SUltiCrossConfigDialog.h"
#include "SUltiCrosshairViewModel.h"
#include "UltiCrosshair.h"
#include "FindProperty.h"

FConstrainedSliderDelegate::FConstrainedSliderDelegate(FUltiCrosshairViewModel* ViewModel, const FString& PropertyPath)
  : ViewModel(ViewModel)
  , PropertyPath(PropertyPath)
  , CachedConstraint(MakeShared<FUltiCrosshairConstraint>(0.0f, 100.0f))
{
  check(ViewModel);
  CacheReferences();
}

void FConstrainedSliderDelegate::CacheReferences()
{
  UUltiCrosshair *Crosshair = ViewModel->GetCrosshair();

  // Always cache the constraint, for when Type is changed.
  CachedConstraint = Crosshair->GetConstraint(PropertyPath);

  // Do nothing if we have the same object
  if (Crosshair == Obj)
  {
    return;
  }

  // Update references to avoid doing this every tick
  Obj = Crosshair;
  Prop = FindPropertyChecked<UNumericProperty>(Obj, PropertyPath, &PropData);
}

float FConstrainedSliderDelegate::Get() const
{
  return CachedConstraint->Normalize(GetRaw());
}

float FConstrainedSliderDelegate::GetRaw() const
{
  if (Prop->IsInteger()) {
    return (float)Prop->GetSignedIntPropertyValue(PropData);
  }

  return Prop->GetFloatingPointPropertyValue(PropData);
}

void FConstrainedSliderDelegate::Set(float Value)
{
  TSharedRef<FUltiCrosshairConstraint> Constraint = CachedConstraint;

  Value = Constraint->Denormalize(Value);

  if (Prop->IsInteger()) {
    Prop->SetIntPropertyValue(PropData, (int64)Value);
  } else {
    Prop->SetFloatingPointPropertyValue(PropData, Value);
  }

  Obj->UpdateTexture();
}

FText FConstrainedSliderDelegate::Text() const
{
  return FText::AsNumber(GetRaw());
}

FUltiCrosshairViewModel::FUltiCrosshairViewModel(SUltiCrossConfigDialog * View)
  : View(View)
  , Crosshair(nullptr)
  , Brush(new FSlateBrush())
{
  // Populate Crosshair Types
  CrosshairTypes.Add(MakeTypeDescriptor(EUltiCrossCrosshairType::Crosshairs, TEXT("Crosshairs")));
  CrosshairTypes.Add(MakeTypeDescriptor(EUltiCrossCrosshairType::Dot, TEXT("Dot")));
  CrosshairTypes.Add(MakeTypeDescriptor(EUltiCrossCrosshairType::Circle, TEXT("Circle")));
  CrosshairTypes.Add(MakeTypeDescriptor(EUltiCrossCrosshairType::Ngon, TEXT("N-gon")));
}

FUltiCrosshairViewModel::~FUltiCrosshairViewModel()
{
  delete Brush;
}

void FUltiCrosshairViewModel::SetCrosshair(UUltiCrosshair* C)
{
  check(C);
  Crosshair = C;
  Brush->SetResourceObject(C->Texture);

  // Let every delegate know so they can update their cached references
  for (const TPair<FString, TSharedRef<FConstrainedSliderDelegate>>& Pair : Delegates)
  {
    Pair.Value->CacheReferences();
  }

  // The view needs to update the selected type when the crosshair seletion is
  // changed
  if (View->CrosshairTypeComboBox.IsValid())
  {
    View->CrosshairTypeComboBox->SetSelectedItem(GetCrosshairTypeDescriptor());
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

FText FUltiCrosshairViewModel::GetTypeText() const
{
  TSharedPtr<FUltiCrosshairTypeDescriptor> Descriptor = GetCrosshairTypeDescriptor();
  if (!Descriptor.IsValid())
  {
    return FText();
  }

  return Descriptor->Text();
}

void FUltiCrosshairViewModel::OnTypeChanged(TSharedPtr<FUltiCrosshairTypeDescriptor> Descriptor, ESelectInfo::Type SelectType)
{
  Crosshair->Type = Descriptor->Type;

  // A change in type may have triggered other constraints
  for (const TPair<FString, TSharedRef<FConstrainedSliderDelegate>>& Pair : Delegates)
  {
    Pair.Value->CacheReferences();
  }

  Crosshair->UpdateTexture();
}

TSharedPtr<FUltiCrosshairTypeDescriptor> FUltiCrosshairViewModel::GetCrosshairTypeDescriptor() const
{
  for (TSharedPtr<FUltiCrosshairTypeDescriptor> Descriptor : CrosshairTypes)
  {
    if (Descriptor->Type == Crosshair->Type)
    {
      return Descriptor;
    }
  }

  return nullptr;
}

int32 FUltiCrosshairViewModel::GetCrosshairTypeDescriptorIndex() const
{
  for (int32 i = 0; i < CrosshairTypes.Num(); ++i)
  {
    if (CrosshairTypes[i]->Type == Crosshair->Type)
    {
      return i;
    }
  }

  return -1;
}

FLinearColor FUltiCrosshairViewModel::GetFillColor() const
{
  return Crosshair->Color.Fill;
}

void FUltiCrosshairViewModel::SetFillColor(FLinearColor Color)
{
  Crosshair->Color.Fill = Color;
  Crosshair->UpdateTexture();
}

FLinearColor FUltiCrosshairViewModel::GetOutlineColor() const
{
  return Crosshair->Color.Outline;
}

void FUltiCrosshairViewModel::SetOutlineColor(FLinearColor Color)
{
  Crosshair->Color.Outline = Color;
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
