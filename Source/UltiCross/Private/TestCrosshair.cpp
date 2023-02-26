#include "UltiCrossPCH.h"
#include "TestCrosshair.h"

TStaticArray<UTexture2D*, 10> UTestCrosshair::TextureSlots;

UTestCrosshair::UTestCrosshair(class FObjectInitializer const & PCIP) : Super(PCIP), TextureSlot(0)
{

}

void UTestCrosshair::DrawCrosshair_Implementation(AUTHUD* TargetHUD, UCanvas* Canvas, AUTWeapon* Weapon, float DeltaTime, const FWeaponCustomizationInfo& CustomizationsToApply)
{
  FCanvasIcon Icon = CrosshairIcon;
  if (TextureSlots[TextureSlot] != nullptr)
  {
    Icon = FCanvasIcon();
    Icon.Texture = TextureSlots[TextureSlot];
    Icon.UL = 32.0f;
    Icon.VL = 32.0f;
  }

  float HUDCrosshairScale = (TargetHUD == nullptr ? 1.0f : TargetHUD->GetCrosshairScale());

  float X = (Canvas->SizeX * 0.5f) - FMath::CeilToFloat(Icon.UL * CustomizationsToApply.CrosshairScaleOverride * HUDCrosshairScale * 0.5f) + OffsetAdjust.X;
  float Y = (Canvas->SizeY * 0.5f) - FMath::CeilToFloat(Icon.VL * CustomizationsToApply.CrosshairScaleOverride * HUDCrosshairScale * 0.5f)  + OffsetAdjust.Y;

  Canvas->DrawColor = CustomizationsToApply.CrosshairColorOverride.ToFColor(false);
  Canvas->DrawIcon(Icon, X, Y, CustomizationsToApply.CrosshairScaleOverride * HUDCrosshairScale);
}

