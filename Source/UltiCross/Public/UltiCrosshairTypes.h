#pragma once

#include "Object.h"

#include "UltiCrosshairTypes.generated.h"

UENUM()
enum class EUltiCrossCrosshairType : uint8
{
    Crosshairs = 0,
    Dot = 1,
    Circle = 2,
    Ngon = 3,
};
