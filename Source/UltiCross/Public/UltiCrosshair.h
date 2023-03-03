#pragma once

#include "Object.h"
#include "UTCrosshair.h"
#include "UltiCrosshairTypes.h"
#include "UltiCrosshairConstraints.h"

#include "UltiCrosshair.generated.h"

USTRUCT()
struct FUltiCrossCrosshairParams
{
  GENERATED_USTRUCT_BODY()

  FUltiCrossCrosshairParams()
  : Count(4)
  , CenterGap(4.0f)
  , Length(8.0f)
  {}

  UPROPERTY()
  uint32 Count;

  UPROPERTY()
  float CenterGap;

  UPROPERTY()
  float Length;
};

USTRUCT()
struct FUltiCrossCircleParams
{
  GENERATED_USTRUCT_BODY()

  FUltiCrossCircleParams()
  : Radius(8.0f)
  {}

  UPROPERTY()
  float Radius;
};

USTRUCT()
struct FUltiCrossNgonParams
{
  GENERATED_USTRUCT_BODY()

  FUltiCrossNgonParams()
  : Edges(3)
  , RadiusX(22.0f)
  , RadiusY(18.0f)
  , OffsetX(0.0f)
  , OffsetY(-2.0f)
  {}

  UPROPERTY()
  uint32 Edges;

  UPROPERTY()
  float RadiusX;

  UPROPERTY()
  float RadiusY;

  UPROPERTY()
  float OffsetX;

  UPROPERTY()
  float OffsetY;
};

USTRUCT()
struct FUltiCrossColors
{
  GENERATED_USTRUCT_BODY()

  FUltiCrossColors()
  : Fill(1.0, 1.0, 1.0, 1.0)
  , Outline(0.0, 0.0, 0.0, 1.0)
  {}

  UPROPERTY()
  FLinearColor Fill;

  UPROPERTY()
  FLinearColor Outline;
};

UCLASS(Config = Game, ConfigDoNotCheckDefaults)
class ULTICROSS_API UUltiCrosshair : public UUTCrosshair
{
  GENERATED_UCLASS_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiCross")
  int UltiCrossSlot;

  /** Crosshair Type */
  UPROPERTY(Config)
  EUltiCrossCrosshairType Type;

  /** Thickness */
  UPROPERTY(Config)
  float Thickness;

  /** Outline Thickness */
  UPROPERTY(Config)
  float Outline;

  /** Rotation */
  UPROPERTY(Config)
  float Rotation;

  /** Colors */
  UPROPERTY(Config)
  FUltiCrossColors Color;

  /** Parameters for Crosshairs type */
  UPROPERTY(Config)
  FUltiCrossCrosshairParams Crosshairs;

  /** Parameters for Circle type */
  UPROPERTY(Config)
  FUltiCrossCircleParams Circle;

  /** Parameters for Ngon type */
  UPROPERTY(Config)
  FUltiCrossNgonParams Ngon;

  UFUNCTION()
  void UpdateTexture();

  UTexture2D *GetTexture() { return Texture; }
  TSharedRef<FUltiCrosshairConstraint> GetConstraint(const FString& PropertyPath);

  void PostInitProperties() override;

  /** Draw the crosshair using our own implementation. */
  void DrawCrosshair_Implementation(AUTHUD* TargetHUD, UCanvas* Canvas, AUTWeapon* Weapon, float DeltaTime, const FWeaponCustomizationInfo& CustomizationsToApply) override;

private:
  UTexture2D* Texture;
};
