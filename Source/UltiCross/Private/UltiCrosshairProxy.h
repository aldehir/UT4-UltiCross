#pragma once

#include "UltiCrosshair.h"

class FUltiCrosshairProxy
{
public:
  FUltiCrosshairProxy();

  void SetCrosshair(UUltiCrosshair* Crosshair);
  UUltiCrosshair* GetCrosshair() const;

  FText GetCrosshairName() const;

  float GetThickness() const;
  void SetThickness(float Value);

  float GetGap() const;
  void SetGap(float Value);

  float GetLength() const;
  void SetLength(float Value);

private:
  UUltiCrosshair* Crosshair;
  UUltiCrosshair* CrosshairCDO;
};