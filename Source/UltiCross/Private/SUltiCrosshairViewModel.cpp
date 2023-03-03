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
  FMemory::Memzero(CachedInstance);
  FMemory::Memzero(CachedCDO);

  CacheReferences();
}

void FConstrainedSliderDelegate::CacheReferences()
{
  UUltiCrosshair *Obj = ViewModel->GetCrosshair();

  // Always cache the constraint, for when Type is changed.
  CachedConstraint = Obj->GetConstraint(PropertyPath);

  // Do nothing if we have the same object
  if (Obj == CachedInstance.Obj)
  {
    return;
  }

  // Update references to avoid doing this every tick
  CachedRef& Inst = CachedInstance;
  CachedRef& CDO = CachedCDO;

  Inst.Obj = Obj;
  Inst.Prop = FindPropertyChecked<UNumericProperty>(Inst.Obj, PropertyPath, &Inst.PropData);

  CDO.Obj = GetMutableDefault<UUltiCrosshair>(Obj->GetClass());
  CDO.Prop = FindPropertyChecked<UNumericProperty>(CDO.Obj, PropertyPath, &CDO.PropData);
}

float FConstrainedSliderDelegate::Get() const
{
  return CachedConstraint->Normalize(GetRaw());
}

float FConstrainedSliderDelegate::GetRaw() const
{
  CachedRef Inst = CachedInstance;

  if (Inst.Prop->IsInteger()) {
    return (float)Inst.Prop->GetSignedIntPropertyValue(Inst.PropData);
  }

  return Inst.Prop->GetFloatingPointPropertyValue(Inst.PropData);
}

void FConstrainedSliderDelegate::Set(float Value)
{
  CachedRef& Inst = CachedInstance;
  TSharedRef<FUltiCrosshairConstraint> Constraint = CachedConstraint;

  Value = Constraint->Denormalize(Value);

  if (Inst.Prop->IsInteger()) {
    Inst.Prop->SetIntPropertyValue(Inst.PropData, (int64)Value);
  } else {
    Inst.Prop->SetFloatingPointPropertyValue(Inst.PropData, Value);
  }

  Inst.Obj->UpdateTexture();
}

FText FConstrainedSliderDelegate::Text() const
{
  return FText::AsNumber(GetRaw());
}

FUltiCrosshairViewModel::FUltiCrosshairViewModel(SUltiCrossConfigDialog * View)
  : View(View)
  , Crosshair(nullptr)
  , CrosshairCDO(nullptr)
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
  CrosshairCDO = GetMutableDefault<UUltiCrosshair>(C->GetClass());
  Brush->SetResourceObject(C->GetTexture());

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
  Crosshair->Type = CrosshairCDO->Type = Descriptor->Type;
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
