#include "UltiCrossPCH.h"
#include "SUltiCrosshairViewModel.h"
#include "UltiCrosshair.h"

SUltiCrosshairViewModel::SUltiCrosshairViewModel(UUltiCrosshair *Crosshair) : Model(Crosshair)
{

}

FText SUltiCrosshairViewModel::GetCrosshairName() const
{
  if (Model != nullptr)
  {
    return Model->CrosshairName;
  }

  return FText();
}