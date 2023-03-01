#pragma once

#include "Object.h"
#include "UTCrosshair.h"
#include "UltiCrosshair.generated.h"

#define ULTICROSS_THICKNESS_MIN 1.0f
#define ULTICROSS_THICKNESS_MAX 11.0f
#define ULTICROSS_THICKNESS_DELTA (ULTICROSS_THICKNESS_MAX - ULTICROSS_THICKNESS_MIN)

UCLASS(Config = Game, ConfigDoNotCheckDefaults)
class ULTICROSS_API UUltiCrosshair : public UUTCrosshair
{
  GENERATED_UCLASS_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiCross")
  int UltiCrossSlot;

  /**
   * Type of Crosshair
   * 
   * Choices:
   * - Crosshairs
   * - Circle
   */
  UPROPERTY(Config)
  FName Type;

  /**
   * Crosshair Thickness
   */
  UPROPERTY(Config)
  float Thickness;

  /**
   * Center gap.
   */
  UPROPERTY(Config)
  float Gap;

  /**
   * Crosshair length. Only valid for Crosshair types.
   */
  UPROPERTY(Config)
  float Length;

  UFUNCTION()
  void UpdateTexture();

  UTexture2D *GetTexture() { return Texture; }

  void PostInitProperties() override;

private:
  UTexture2D* Texture;
};