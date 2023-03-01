#include "UltiCrossPCH.h"

#include "UltiCrosshairProxy.h"

FUltiCrosshairProxy::FUltiCrosshairProxy()
  : Crosshair(nullptr), CrosshairCDO(nullptr)
{
}

void FUltiCrosshairProxy::SetCrosshair(UUltiCrosshair* C)
{
  Crosshair = C;
  if (C != nullptr)
  {
    CrosshairCDO = GetMutableDefault<UUltiCrosshair>(C->GetClass());
  }
}

UUltiCrosshair* FUltiCrosshairProxy::GetCrosshair() const
{
  return Crosshair;
}

FText FUltiCrosshairProxy::GetCrosshairName() const
{
  return Crosshair->CrosshairName;
}

float FUltiCrosshairProxy::GetThickness() const
{
  return Crosshair->Thickness;
}

void FUltiCrosshairProxy::SetThickness(float Value)
{
  Crosshair->Thickness = CrosshairCDO->Thickness = Value;
  Crosshair->UpdateTexture();
}

float FUltiCrosshairProxy::GetGap() const
{
  return Crosshair->Gap;
}

void FUltiCrosshairProxy::SetGap(float Value)
{
  Crosshair->Gap = CrosshairCDO->Gap = Value;
  Crosshair->UpdateTexture();
}

float FUltiCrosshairProxy::GetLength() const
{
  return Crosshair->Length;
}

void FUltiCrosshairProxy::SetLength(float Value)
{
  Crosshair->Length = CrosshairCDO->Length = Value;
  Crosshair->UpdateTexture();
}