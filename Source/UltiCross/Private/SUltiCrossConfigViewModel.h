#pragma once

#include "Core.h"

class UUltiCrosshair;

/**
 * Crosshair Configuration ViewModel.
 */
class SUltiCrossConfigViewModel
{
public:
  SUltiCrossConfigViewModel(UUltiCrosshair *Crosshair);

  inline void SetModel(UUltiCrosshair* Crosshair) { Model = Crosshair; }
  inline UUltiCrosshair* GetModel() const { return Model; }

  FText GetCrosshairName() const;

private:
  UUltiCrosshair* Model;
};