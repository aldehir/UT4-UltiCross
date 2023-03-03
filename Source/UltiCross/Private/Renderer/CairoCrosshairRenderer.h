#pragma once

#include "RenderContext.h"

#include "cairo/cairo.h"

class UUltiCrosshair;

class FCairoRenderContext : public FRenderContext
{
public:
  FCairoRenderContext(UTexture2D* Tex);

  void Begin() override;
  void End() override;

  cairo_surface_t* CairoSurface;
  cairo_t* CairoContext;
};

enum class ECairoOperator : uint8 {
  Source = CAIRO_OPERATOR_SOURCE,
  Over = CAIRO_OPERATOR_OVER
};

const int RenderStroke = 0x01;
const int RenderFill = 0x02;
const int RenderAll = RenderStroke | RenderFill;

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
  void SetOperator(ECairoOperator Op);

  void Fill(const FLinearColor& Color, bool bPreserve = false);
  void Stroke(const FLinearColor& Color, bool bPreserve = false);

protected:
  FCairoRenderContext* Ctx;
};

class FCairoCrosshairRenderer
{
public:
  FCairoCrosshairRenderer();

  void Render(UUltiCrosshair *Crosshair);

private:
  void RenderBackground(FCairoRenderContext *Ctx);
  void RenderCrosshairs(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair, int Render = RenderAll);
  void RenderCircle(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair);
  void RenderNgon(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair);
  void RenderDot(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair);
};
