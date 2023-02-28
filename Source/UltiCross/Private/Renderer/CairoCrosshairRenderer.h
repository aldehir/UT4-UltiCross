#pragma once

#include "RenderContext.h"

#include "cairo/cairo.h"

class FCairoRenderContext : public FRenderContext
{
public:
  FCairoRenderContext(UTexture2D* Tex);

  void Begin() override;
  void End() override;

  cairo_surface_t* CairoSurface;
  cairo_t* CairoContext;
};

/**
 * Saves and restores the Cairo context within local scope.
 */
class FCairoSession
{
public:
  FCairoSession(FCairoRenderContext *Ctx);
  ~FCairoSession();

  void Clear();

  void MoveTo(const FVector2D& Point);
  void LineTo(const FVector2D& Point);
  void RelativeLineTo(const FVector2D& Vec);
  void ClosePath();

  void Translate(const FVector2D& V);
  void Rotate(float Angle);
  void RotateAround(const FVector2D& P, float Angle);

  void SetLineWidth(float Width);

  void Fill(const FLinearColor& Color, bool bPreserve = false);
  void Stroke(const FLinearColor& Color, bool bPreserve = false);

protected:
  FCairoRenderContext* Ctx;
};

class FCairoCrosshairRenderer
{
public:
  FCairoCrosshairRenderer();

  FRenderContext* CreateContext(UTexture2D* Texture);

  void Clear(FRenderContext *Ctx);
  void RenderCrosshairs(FRenderContext *Ctx, float Thickness, float Length, float Gap, const FLinearColor& Fill, const FLinearColor& Outline);
};