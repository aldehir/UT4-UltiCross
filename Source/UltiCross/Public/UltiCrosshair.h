#pragma once

#include "Object.h"
#include "UTCrosshair.h"
#include "UltiCrosshair.generated.h"

#define ULTICROSS_THICKNESS_MIN 1.0f
#define ULTICROSS_THICKNESS_MAX 11.0f
#define ULTICROSS_THICKNESS_DELTA (ULTICROSS_THICKNESS_MAX - ULTICROSS_THICKNESS_MIN)

/** Constraints */
namespace UltiCrosshairConstraint
{
  extern const float ThicknessMin;
  extern const float ThicknessMax;
  extern const float ThicknessResolution;

  extern const float GapMin;
  extern const float GapMax;
  extern const float GapResolution;

  extern const float LengthMin;
  extern const float LengthMax;
  extern const float LengthResolution;
};

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