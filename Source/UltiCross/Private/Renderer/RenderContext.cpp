#include "../UltiCrossPCH.h"
#include "RenderContext.h"

FRenderContext::FRenderContext(UTexture2D* Tex) : Texture(Tex), ImageData(nullptr), RawData(nullptr)
{
  if (Texture != nullptr)
  {
    ImageData = &Texture->PlatformData->Mips[0].BulkData;
  }
}

int32 FRenderContext::SizeX()
{
  if (Texture != nullptr)
  {
    return Texture->PlatformData->SizeX;
  }
  
  return 0;
}

int32 FRenderContext::SizeY()
{
  if (Texture != nullptr)
  {
    return Texture->PlatformData->SizeY;
  }
  
  return 0;
}

void FRenderContext::Begin()
{
  RawData = (uint8*)ImageData->Lock(LOCK_READ_WRITE);
}

void FRenderContext::End()
{
  ImageData->Unlock();
  RawData = nullptr;

  Texture->UpdateResource();
}