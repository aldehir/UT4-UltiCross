#pragma once

#include "Object.h"
#include "UTCrosshair.h"
#include "TestCrosshair.generated.h"

UCLASS()
class ULTICROSS_API UTestCrosshair : public UUTCrosshair
{
  GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crosshair)
    FName ExampleProperty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Crosshair)
    int TextureSlot;

    void DrawCrosshair_Implementation(AUTHUD* TargetHUD, UCanvas* Canvas, AUTWeapon* Weapon, float DeltaTime, const FWeaponCustomizationInfo& CustomizationsToApply) override;

    static TStaticArray<UTexture2D*, 10> TextureSlots;
};
