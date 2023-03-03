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
  CrosshairIcon.Texture = Texture;
  CrosshairIcon.UL = Texture->GetSizeX();
  CrosshairIcon.VL = Texture->GetSizeY();

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