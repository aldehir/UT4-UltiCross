#include "UltiCrossPCH.h"
#include "UltiCrosshair.h"
#include "Fixtures.h"

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

  CrosshairIcon.Texture = CreateExampleCrosshairTexture();
  CrosshairIcon.UL = 64.0f;
  CrosshairIcon.VL = 64.0f;
}