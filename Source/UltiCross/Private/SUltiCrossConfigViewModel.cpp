#include "UltiCrossPCH.h"
#include "SUltiCrossConfigViewModel.h"
#include "UltiCrosshair.h"

SUltiCrossConfigViewModel::SUltiCrossConfigViewModel(UUltiCrosshair *Crosshair) : Model(Crosshair)
{

}

FText SUltiCrossConfigViewModel::GetCrosshairName() const
{
  if (Model != nullptr)
  {
    return Model->CrosshairName;
  }

  return FText();
}