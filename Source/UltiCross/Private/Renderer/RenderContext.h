#pragma once

#include "Core.h"
#include "Engine.h"
#include "Object.h"

class FRenderContext
{
public:
  FRenderContext(UTexture2D* Texture);

  /** Prepares the texture for rendering. */
  virtual void Begin();

  /** Closes resources. */
  virtual void End();

  /** Returns the width of the underlying texture. */
  virtual int32 SizeX();

  /** Returns the height of the underlying texture. */
  virtual int32 SizeY();

  /** Pointer to raw data set when Begin() is called, otherwise it is nullptr. */
  uint8* RawData;

private:
  UTexture2D* Texture;
  FByteBulkData* ImageData;
};