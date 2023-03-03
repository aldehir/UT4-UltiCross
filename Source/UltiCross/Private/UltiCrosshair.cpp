#include "UltiCrossPCH.h"
#include "UltiCross.h"

#include "Renderer/CairoCrosshairRenderer.h"

UUltiCrosshair::UUltiCrosshair(class FObjectInitializer const & PCIP) : Super(PCIP)
{
}

void UUltiCrosshair::PostInitProperties()
{
  Super::PostInitProperties();
  Texture = UTexture2D::CreateTransient(64, 64);
  UpdateTexture();
}

void UUltiCrosshair::UpdateTexture()
{
  FCairoCrosshairRenderer Renderer;
  Renderer.Render(this);
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

  Canvas->DrawTile(Texture, X, Y, Width, Height, 0.0f, 0.0f, Width, Height);
}
