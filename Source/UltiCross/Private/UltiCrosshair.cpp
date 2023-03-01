#include "UltiCrossPCH.h"
#include "UltiCrosshair.h"

#include "Renderer/CairoCrosshairRenderer.h"

const float UltiCrosshairConstraint::ThicknessMin = 1.0f;
const float UltiCrosshairConstraint::ThicknessMax = 10.0f;
const float UltiCrosshairConstraint::ThicknessResolution = 1.0f;

const float UltiCrosshairConstraint::GapMin = 0.0f;
const float UltiCrosshairConstraint::GapMax = 20.0;
const float UltiCrosshairConstraint::GapResolution = 1.0f;

const float UltiCrosshairConstraint::LengthMin = 1.0f;
const float UltiCrosshairConstraint::LengthMax = 40.0f;
const float UltiCrosshairConstraint::LengthResolution = 1.0f;

UUltiCrosshair::UUltiCrosshair(class FObjectInitializer const & PCIP) : Super(PCIP)
{
  Type = FName(TEXT("Crosshairs"));
  Thickness = 1.0f;
  Gap = 4.0f;
  Length = 8.0f;
}

void UUltiCrosshair::PostInitProperties()
{
  Super::PostInitProperties();

  UE_LOG(LogUltiCross, Log, TEXT("Crosshair Loaded: Name=%s Type=%s Thickness=%f Gap=%f Length=%f"),
    *CrosshairName.ToString(), *Type.ToString(), Thickness, Gap, Length);

  Texture = UTexture2D::CreateTransient(64, 64);
  CrosshairIcon.Texture = Texture;
  CrosshairIcon.UL = Texture->GetSizeX();
  CrosshairIcon.VL = Texture->GetSizeY();

  UpdateTexture();
}

void UUltiCrosshair::UpdateTexture()
{
  FCairoCrosshairRenderer Renderer;
  FCairoRenderContext RenderContext(Texture);

  RenderContext.Begin();
  Renderer.Clear(&RenderContext);
  Renderer.RenderCrosshairs(&RenderContext, Thickness, Length, Gap, FLinearColor::White, FLinearColor::Black);
  RenderContext.End();
}