#pragma once

#include "Core.h"

class UUltiCrosshair;

/**
 * Crosshair Configuration ViewModel.
 */
class SUltiCrosshairViewModel
{
public:
  SUltiCrosshairViewModel(UUltiCrosshair *Crosshair);

  inline void SetModel(UUltiCrosshair* Crosshair) { Model = Crosshair; }
  inline UUltiCrosshair* GetModel() const { return Model; }

  FText GetCrosshairName() const;

private:
  UUltiCrosshair* Model;
};