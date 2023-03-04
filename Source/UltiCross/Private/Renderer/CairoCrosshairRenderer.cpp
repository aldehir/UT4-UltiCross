#include "../UltiCrossPCH.h"

#include "CairoCrosshairRenderer.h"

FTextureLocker::FTextureLocker(UTexture2D *Texture) : Texture(Texture)
{
  check(Texture);
  BulkData = &Texture->PlatformData->Mips[0].BulkData;
  Data = (void*)BulkData->Lock(LOCK_READ_WRITE);
}

FTextureLocker::~FTextureLocker()
{
  BulkData->Unlock();
  Texture->UpdateResource();
}

FCairoSurface::FCairoSurface(void* Data, cairo_format_t Format, int Width, int Height)
{
  int stride = cairo_format_stride_for_width(Format, Width);
  Surface = cairo_image_surface_create_for_data((uint8*)Data, Format, Width, Height, stride);
}

FCairoSurface::~FCairoSurface() { cairo_surface_destroy(Surface); }

FCairoContext::FCairoContext(FCairoSurface& Surface) { Context = cairo_create(Surface.GetSurface()); }
FCairoContext::~FCairoContext() { cairo_destroy(Context); }

void FCairoContext::Save() { cairo_save(Context); }
void FCairoContext::Restore() { cairo_restore(Context); }

void FCairoContext::Paint() { cairo_paint(Context); }

void FCairoContext::MoveTo(const FVector2D& Point) { cairo_move_to(Context, Point.X, Point.Y); }
void FCairoContext::LineTo(const FVector2D& Point) { cairo_line_to(Context, Point.X, Point.Y); }
void FCairoContext::RelLineTo(const FVector2D& Vec) { cairo_rel_line_to(Context, Vec.X, Vec.Y); }
void FCairoContext::Arc(const FVector2D& Center, float Radius, float StartAngle, float EndAngle)
{
  cairo_arc(Context, Center.X, Center.Y, Radius, StartAngle, EndAngle);
}
void FCairoContext::ClosePath() { cairo_close_path(Context); }

void FCairoContext::Translate(const FVector2D& V) { cairo_translate(Context, V.X, V.Y); }
void FCairoContext::Rotate(float Angle) { cairo_rotate(Context, Angle); }

void FCairoContext::SetSourceRGBA(const FLinearColor& Color) { cairo_set_source_rgba(Context, Color.R, Color.G, Color.B, Color.A); }
void FCairoContext::SetLineWidth(float Width) { cairo_set_line_width(Context, Width); }
void FCairoContext::SetOperator(cairo_operator_t Op) { cairo_set_operator(Context, Op); }

void FCairoContext::Fill() { cairo_fill(Context); }
void FCairoContext::Stroke() { cairo_stroke(Context); }
void FCairoContext::FillPreserve() { cairo_fill_preserve(Context); }
void FCairoContext::StrokePreserve() { cairo_stroke(Context); }

FCairoCrosshairRenderer::FCairoCrosshairRenderer()
{}

void FCairoCrosshairRenderer::Render(UUltiCrosshair *Crosshair)
{
  FTextureLocker Locker(Crosshair->Texture);

  int Width = Crosshair->Texture->GetSurfaceWidth();
  int Height = Crosshair->Texture->GetSurfaceHeight();

  FCairoSurface Surface(Locker.GetData(), CAIRO_FORMAT_ARGB32, Width, Height);
  FCairoContext Cairo(Surface);

  FRenderContext Ctx;
  FMemory::Memzero(Ctx);

  Ctx.Crosshair = Crosshair;
  Ctx.Width = Width;
  Ctx.Height = Height;
  Ctx.Center = FVector2D(Width * 0.5f, Height * 0.5f);
  Ctx.bOffCenter = false;
  Ctx.Render = RenderAll;

  RenderBackground(Cairo, Ctx);

  Ctx.Render = RenderStroke;
  RenderCrosshairs(Cairo, Ctx);
  RenderCircle(Cairo, Ctx);
  RenderNgon(Cairo, Ctx);
  RenderDot(Cairo, Ctx);

  Ctx.Render = RenderFill;
  RenderCrosshairs(Cairo, Ctx);
  RenderCircle(Cairo, Ctx);
  RenderNgon(Cairo, Ctx);
  RenderDot(Cairo, Ctx);
}

void FCairoCrosshairRenderer::RenderBackground(FCairoContext& Cairo, FRenderContext& Ctx)
{
  Cairo.SetSourceRGBA(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
  Cairo.SetOperator(CAIRO_OPERATOR_SOURCE);
  Cairo.Paint();
}

void FCairoCrosshairRenderer::RenderCrosshairs(FCairoContext& Cairo, FRenderContext& Ctx)
{
  UUltiCrosshair* Crosshair = Ctx.Crosshair;
  if (Crosshair->Type != EUltiCrossCrosshairType::Crosshairs) return;

  FUltiCrossCrosshairParams Params(Crosshair->Crosshairs);

  float Outline = Crosshair->Outline;
  float Rotation = FMath::Clamp(Crosshair->Rotation, 0.0f, 360.0f);
  float Thickness = Params.Thickness;
  float CenterGap = Params.CenterGap;

  // Do nothing if length = 0
  if (FMath::IsNearlyZero(Params.Length)) return;

  // Pre-compute angles
  TArray<float> Angles;

  float AngleIncrement = 360.0f / Params.Count;

  for (float Angle = 0.0f; Angle < 360.5f; Angle += AngleIncrement)
  {
    float Wrapped = FMath::Fmod(Rotation + Angle, 360.0f);

    // If any angle is a multiple of 90 and thickness == 0.5, then we want to offset
    // the center by 1 in both axes to render a pixel perfect crosshair.
    Ctx.bOffCenter = FMath::IsNearlyZero(FMath::Fmod(Wrapped, 90.0f)) && FMath::IsNearlyEqual(Thickness, 0.5f);

    Angles.Add(Wrapped);
  }

  FVector2D Center = Ctx.Center;
  if (Ctx.bOffCenter)
  {
    Center = Center - 0.5f;
    CenterGap = CenterGap - 0.5f;
  }

  for (const float Angle : Angles)
  {
    Cairo.Save();

    // Rotate about the center
    Cairo.Translate(Center);
    Cairo.Rotate(FMath::DegreesToRadians(Angle));

    FVector2D OutlineVec(0, Outline);
    FVector2D StartVec(0, -1 * CenterGap);
    FVector2D LengthVec(0, Params.Length);

    // Use the source operator for both the stroke and fill
    Cairo.SetOperator(CAIRO_OPERATOR_SOURCE);

    // Draw the outline stroke first
    if (Ctx.Render & RenderStroke)
    {
      Cairo.MoveTo(StartVec + OutlineVec);
      Cairo.RelLineTo((-1 * LengthVec) - (2 * OutlineVec));
      Cairo.SetLineWidth(2.0f * (Outline + Thickness));
      Cairo.SetSourceRGBA(Crosshair->Color.Outline);
      Cairo.Stroke();
    }

    // Draw the fill
    if (Ctx.Render & RenderFill)
    {
      Cairo.MoveTo(StartVec);
      Cairo.RelLineTo(-1 * LengthVec);
      Cairo.SetLineWidth(2.0f * Thickness);
      Cairo.SetSourceRGBA(Crosshair->Color.Fill);
      Cairo.Stroke();
    }

    Cairo.Restore();
  }
}

void FCairoCrosshairRenderer::RenderCircle(FCairoContext& Cairo, FRenderContext& Ctx)
{
  UUltiCrosshair* Crosshair = Ctx.Crosshair;
  if (Crosshair->Type != EUltiCrossCrosshairType::Circle) return;

  float Outline = Crosshair->Outline;
  float Thickness = Crosshair->Circle.Thickness;
  float Radius = Crosshair->Circle.Radius;

  if (FMath::IsNearlyZero(Radius)) return;

  // Adjust radius for better pixel alignment
  FVector2D Center = Ctx.Center;
  if (Ctx.bOffCenter)
  {
    Center = Center - 0.5f;
    if (FMath::RoundToInt(Thickness) % 2 == 0) {
      Radius = Radius - 0.5f;
    }
  }
  else
  {
    if (FMath::RoundToInt(Thickness) % 2 == 1) {
      Radius = Radius - 0.5f;
    }
  }

  Cairo.Save();

  Cairo.Arc(Center, Radius, FMath::DegreesToRadians(0.0f), FMath::DegreesToRadians(360.0f));
  Cairo.ClosePath();

  Cairo.SetOperator(CAIRO_OPERATOR_SOURCE);

  if (Ctx.Render & RenderStroke && !FMath::IsNearlyZero(Outline))
  {
    Cairo.SetLineWidth((2.0f * Outline) + Thickness);
    Cairo.SetSourceRGBA(Crosshair->Color.Outline);
    Cairo.StrokePreserve();
  }

  if (Ctx.Render & RenderFill)
  {
    Cairo.SetLineWidth(Thickness);
    Cairo.SetSourceRGBA(Crosshair->Color.Fill);
    Cairo.Stroke();
  }

  Cairo.Restore();
}

void FCairoCrosshairRenderer::RenderNgon(FCairoContext& Cairo, FRenderContext& Ctx)
{
  UUltiCrosshair* Crosshair = Ctx.Crosshair;
  if (Crosshair->Type != EUltiCrossCrosshairType::Circle) return;
}

void FCairoCrosshairRenderer::RenderDot(FCairoContext& Cairo, FRenderContext& Ctx)
{
  UUltiCrosshair* Crosshair = Ctx.Crosshair;

  float Outline = Crosshair->Outline;
  float Radius = Crosshair->DotRadius;

  if (FMath::IsNearlyZero(Radius)) return;

  FVector2D Center = Ctx.Center;
  if (Ctx.bOffCenter)
  {
    Center = Center - 0.5f;
    Radius = Radius - 0.5f;
  }

  Cairo.Save();

  Cairo.Arc(Center, Radius, FMath::DegreesToRadians(0.0f), FMath::DegreesToRadians(360.0f));
  Cairo.ClosePath();

  Cairo.SetOperator(CAIRO_OPERATOR_SOURCE);

  if (Ctx.Render & RenderStroke && !FMath::IsNearlyZero(Outline))
  {
    Cairo.SetLineWidth(2.0f * Outline);
    Cairo.SetSourceRGBA(Crosshair->Color.Outline);
    Cairo.StrokePreserve();
  }

  if (Ctx.Render & RenderFill)
  {
    Cairo.SetSourceRGBA(Crosshair->Color.Fill);
    Cairo.Fill();
  }

  Cairo.Restore();
}
