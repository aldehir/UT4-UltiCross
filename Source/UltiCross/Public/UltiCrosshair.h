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
  : Thickness(1.0f)
  , Count(4)
  , CenterGap(4.0f)
  , Length(8.0f)
  {}

  UPROPERTY()
  float Thickness;

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
  : Thickness(2.0f)
  , Radius(8.0f)
  {}

  UPROPERTY()
  float Thickness;

  UPROPERTY()
  float Radius;
};

USTRUCT()
struct FUltiCrossNgonParams
{
  GENERATED_USTRUCT_BODY()

  FUltiCrossNgonParams()
  : Thickness(1.0f)
  , Edges(3)
  , RadiusX(22.0f)
  , RadiusY(18.0f)
  , OffsetX(0.0f)
  , OffsetY(-2.0f)
  {}

  UPROPERTY()
  float Thickness;

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

USTRUCT()
struct FUltiCrossTransform
{
  GENERATED_USTRUCT_BODY()

  FUltiCrossTransform()
  : Rotate(0.0f)
  , Translate(0.0f, 0.0f)
  {}

  UPROPERTY()
  float Rotate;

  UPROPERTY()
  FVector2D Translate;
};

UCLASS(Config = UltiCross, ConfigDoNotCheckDefaults)
class ULTICROSS_API UUltiCrosshair : public UUTCrosshair
{
  GENERATED_UCLASS_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiCross")
  int UltiCrossSlot;

  /** User-defined Name */
  UPROPERTY(Config)
  FString UserDefinedName;

  /** Crosshair Type */
  UPROPERTY(Config)
  EUltiCrossCrosshairType Type;

  /** Outline Thickness */
  UPROPERTY(Config)
  float Outline;

  /** Dot Radius */
  UPROPERTY(Config)
  float DotRadius;

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

  /** Global Transformations */
  UPROPERTY(Config)
  FUltiCrossTransform Transform;

  /** Generated Crosshair texture */
  UPROPERTY(Transient)
  UTexture2D* Texture;

  UFUNCTION()
  void UpdateTexture();

  void InitializeName();
  FText GetUserDefinedNameAsText() const;
  void SetUserDefinedNameFromText(const FText& Name);

  /**
   * Copies the crosshair parameters from Other to this and invokes
   * UpdateTexture().
   */
  void CopyCrosshairParameters(UUltiCrosshair* Other);

  TSharedRef<FUltiCrosshairConstraint> GetConstraint(const FString& PropertyPath);

  virtual void PostInitProperties() override;

  /** Draw the crosshair using our own implementation. */
  void DrawCrosshair_Implementation(AUTHUD* TargetHUD, UCanvas* Canvas, AUTWeapon* Weapon, float DeltaTime, const FWeaponCustomizationInfo& CustomizationsToApply) override;

  static FString GetIniFilename();

  void SaveCrosshair();
  void LoadCrosshair();

};

UCLASS(Blueprintable)
class ULTICROSS_API UUltiCrosshairBundle : public UObject
{
  GENERATED_UCLASS_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UltiCross")
  TArray<TSubclassOf<UUltiCrosshair>> Crosshairs;
};
