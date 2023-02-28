#include "UltiCrossPCH.h"
#include "TestCrosshair.h"
#include "Fixtures.h"

UTestCrosshair::UTestCrosshair(class FObjectInitializer const & PCIP) : Super(PCIP)
{
}

void UTestCrosshair::PostInitProperties()
{
  Super::PostInitProperties();

  CrosshairIcon.Texture = CreateExampleCrosshairTexture();
  CrosshairIcon.UL = 64.0f;
  CrosshairIcon.VL = 64.0f;
}