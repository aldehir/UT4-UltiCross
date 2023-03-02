#include "UltiCrossPCH.h"
#include "UltiCrosshair.h"

#include "Renderer/CairoCrosshairRenderer.h"

const float UltiCrosshairConstraint::ThicknessMin = 0.5f;
const float UltiCrosshairConstraint::ThicknessMax = 10.0f;
const float UltiCrosshairConstraint::ThicknessResolution = 0.5f;

const float UltiCrosshairConstraint::GapMin = 0.0f;
const float UltiCrosshairConstraint::GapMax = 20.0;
const float UltiCrosshairConstraint::GapResolution = 1.0f;

const float UltiCrosshairConstraint::LengthMin = 1.0f;
const float UltiCrosshairConstraint::LengthMax = 40.0f;
const float UltiCrosshairConstraint::LengthResolution = 1.0f;

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