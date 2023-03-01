#pragma once

#include "Object.h"
#include "UTCrosshair.h"
#include "UltiCrosshair.generated.h"

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

  void PostInitProperties() override;
};