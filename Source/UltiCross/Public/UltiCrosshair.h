#pragma once

#include "Object.h"
#include "UTCrosshair.h"
#include "UltiCrosshair.generated.h"


UCLASS()
class ULTICROSS_API UUltiCrosshair : public UUTCrosshair
{
    GENERATED_UCLASS_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UltiCross)
    int UltiCrossSlot;
};