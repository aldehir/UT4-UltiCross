#include "UltiCrossPCH.h"
#include "UltiCrosshair.h"

#include "Renderer/CairoCrosshairRenderer.h"

UUltiCrosshair::UUltiCrosshair(class FObjectInitializer const & PCIP) : Super(PCIP)
{
}

void UUltiCrosshair::PostInitProperties()
{
  Super::PostInitProperties();

  if (HasAnyFlags(RF_ClassDefaultObject))
  {
    // Load configuration if CDO
    LoadCrosshair();
  }
  else
  {
    // Only instances need a texture
    Texture = UTexture2D::CreateTransient(192, 192);
    UpdateTexture();
  }

  InitializeName();
}

void UUltiCrosshair::InitializeName()
{
  // Default to the Crosshair Name if empty
  if (UserDefinedName.IsEmpty())
  {
    UserDefinedName = CrosshairName.ToString();
    return;
  }

  // Otherwise, set CrosshairName to UserDefinedName for the UI
  CrosshairName = FText::FromString(UserDefinedName);
}

FText UUltiCrosshair::GetUserDefinedNameAsText() const
{
  return FText::FromString(UserDefinedName);
}

void UUltiCrosshair::SetUserDefinedNameFromText(const FText& Name)
{
  UserDefinedName = Name.ToString();
  CrosshairName = Name;
}

void UUltiCrosshair::UpdateTexture()
{
  FCairoCrosshairRenderer Renderer;
  Renderer.Render(this);
}

void UUltiCrosshair::CopyCrosshairParameters(UUltiCrosshair* Other)
{
  CrosshairName = Other->CrosshairName;
  UserDefinedName = Other->UserDefinedName;
  Type = Other->Type;
  Outline = Other->Outline;
  DotRadius = Other->DotRadius;
  Color = Other->Color;
  Crosshairs = Other->Crosshairs;
  Circle = Other->Circle;
  Ngon = Other->Ngon;
  Transform = Other->Transform;
}

TSharedRef<FUltiCrosshairConstraint> UUltiCrosshair::GetConstraint(const FString& PropertyPath)
{
  TSharedRef<FUltiCrosshairConstraintMap> Map = FUltiCross::Get()->GetConstraintMapForPath(PropertyPath);
  return Map->Get(Type);
}

void UUltiCrosshair::DrawCrosshair_Implementation(AUTHUD* TargetHUD, UCanvas* Canvas, AUTWeapon* Weapon, float DeltaTime, const FWeaponCustomizationInfo& CustomizationsToApply)
{
  float HUDCrosshairScale = (TargetHUD == nullptr ? 1.0f : TargetHUD->GetCrosshairScale());

  float Width = Texture->GetSurfaceWidth();
  float Height = Texture->GetSurfaceHeight();

  float X = (Canvas->SizeX * 0.5f) - FMath::CeilToFloat(Width * 0.5f);
  float Y = (Canvas->SizeY * 0.5f) - FMath::CeilToFloat(Height * 0.5f);

  Canvas->DrawColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f).ToFColor(false);
  Canvas->DrawTile(Texture, X, Y, Width * HUDCrosshairScale, Height * HUDCrosshairScale, 0.0f, 0.0f, Width, Height);
}

FString UUltiCrosshair::GetIniFilename()
{
  return FPaths::Combine(*FPaths::GeneratedConfigDir(), FPlatformProperties::PlatformName(), TEXT("UltiCross.ini"));
}


void UUltiCrosshair::SaveCrosshair()
{
  SaveConfig(CPF_Config, *GetIniFilename());
}

void UUltiCrosshair::LoadCrosshair()
{
  LoadConfig(GetClass(), *GetIniFilename());
}

UUltiCrosshairBundle::UUltiCrosshairBundle(FObjectInitializer const & PCIP) : Super(PCIP)
{}
