#pragma once

#include "RenderContext.h"

#include "cairo/cairo.h"

class UUltiCrosshair;

class FTextureLocker
{
public:
  FTextureLocker(UTexture2D* Texture);
  ~FTextureLocker();

  void* GetData() { return Data; }

private:
  UTexture2D* Texture;
  FByteBulkData* BulkData;
  void* Data;
};

class FCairoSurface
{
public:
  FCairoSurface(void* Data, cairo_format_t Format, int Width, int Height);
  ~FCairoSurface();

  cairo_surface_t* GetSurface() { return Surface; }

private:
  cairo_surface_t* Surface;
};

class FCairoContext
{
public:
  FCairoContext(FCairoSurface& Surface);
  ~FCairoContext();

  void Save();
  void Restore();

  void Paint();

  void MoveTo(const FVector2D& Point);
  void LineTo(const FVector2D& Point);
  void RelLineTo(const FVector2D& Vec);
  void ClosePath();

  void Translate(const FVector2D& V);
  void Rotate(float Angle);

  void SetSourceRGBA(const FLinearColor& Color);
  void SetLineWidth(float Width);
  void SetOperator(cairo_operator_t Op);

  void Fill();
  void Stroke();
  void FillPreserve();
  void StrokePreserve();

private:
  cairo_t* Context;
};

const int RenderStroke = 0x01;
const int RenderFill = 0x02;
const int RenderAll = RenderStroke | RenderFill;

class FCairoCrosshairRenderer
{
public:
  FCairoCrosshairRenderer();

  void Render(UUltiCrosshair *Crosshair);

private:
  void RenderBackground(FCairoContext& Ctx);
  void RenderCrosshairs(FCairoContext& Ctx, UUltiCrosshair *Crosshair, int Render = RenderAll);
  void RenderCircle(FCairoContext& Ctx, UUltiCrosshair *Crosshair);
  void RenderNgon(FCairoContext& Ctx, UUltiCrosshair *Crosshair);
  void RenderDot(FCairoContext& Ctx, UUltiCrosshair *Crosshair);
};
