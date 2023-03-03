#include "../UltiCrossPCH.h"

#include "UltiCrosshair.h"

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
  FCairoContext Ctx(Surface);

  RenderBackground(Ctx);

  RenderCrosshairs(Ctx, Crosshair, RenderStroke);
  RenderCrosshairs(Ctx, Crosshair, RenderFill);

  RenderCircle(Ctx, Crosshair);
  RenderNgon(Ctx, Crosshair);
  RenderDot(Ctx, Crosshair);
}

void FCairoCrosshairRenderer::RenderBackground(FCairoContext& Ctx)
{
  Ctx.SetSourceRGBA(FLinearColor(0.0f, 0.0f, 0.0f, 0.0f));
  Ctx.SetOperator(CAIRO_OPERATOR_SOURCE);
  Ctx.Paint();
}

void FCairoCrosshairRenderer::RenderCrosshairs(FCairoContext& Ctx, UUltiCrosshair *Crosshair, int Render)
{
  if (Crosshair->Type != EUltiCrossCrosshairType::Crosshairs) return;

  int Width = Crosshair->Texture->GetSurfaceWidth();
  int Height = Crosshair->Texture->GetSurfaceHeight();

  bool bOffsetByOne = false;

  float Thickness = Crosshair->Thickness;
  float Outline = Crosshair->Outline;
  float Rotation = FMath::Clamp(Crosshair->Rotation, 0.0f, 360.0f);

  FUltiCrossCrosshairParams Params(Crosshair->Crosshairs);

  FVector2D Center(Width * 0.5f, Height * 0.5f);

  // Pre-compute angles
  TArray<float> Angles;

  float AngleIncrement = 360.0f / Params.Count;

  for (float Angle = 0.0f; Angle < 360.5f; Angle += AngleIncrement)
  {
    float Wrapped = FMath::Fmod(Rotation + Angle, 360.0f);

    // If any angle is a multiple of 90 and thickness == 0.5, then we want to offset
    // the center by 1 in both axes to render a pixel perfect crosshair.
    bOffsetByOne = FMath::IsNearlyZero(FMath::Fmod(Wrapped, 90.0f)) && FMath::IsNearlyEqual(Thickness, 0.5f);

    Angles.Add(Wrapped);
  }

  if (bOffsetByOne)
  {
    Center -= FVector2D(0.5f, 0.5f);
    Params.CenterGap -= 0.5f;
  }

  for (const float Angle : Angles)
  {
    Ctx.Save();

    // Rotate about the center
    Ctx.Translate(Center);
    Ctx.Rotate(FMath::DegreesToRadians(Angle));

    FVector2D OutlineVec(0, Outline);
    FVector2D StartVec(0, -1 * Params.CenterGap);
    FVector2D LengthVec(0, Params.Length);

    // Use the source operator for both the stroke and fill
    Ctx.SetOperator(CAIRO_OPERATOR_SOURCE);

    // Draw the outline stroke first
    if (Render & RenderStroke)
    {
      Ctx.MoveTo(StartVec + OutlineVec);
      Ctx.RelLineTo((-1 * LengthVec) - (2 * OutlineVec));
      Ctx.SetLineWidth(2.0f * (Outline + Thickness));
      Ctx.SetSourceRGBA(Crosshair->Color.Outline);
      Ctx.Stroke();
    }

    // Draw the fill
    if (Render & RenderFill)
    {
      Ctx.MoveTo(StartVec);
      Ctx.RelLineTo(-1 * LengthVec);
      Ctx.SetLineWidth(2.0f * Thickness);
      Ctx.SetSourceRGBA(Crosshair->Color.Fill);
      Ctx.Stroke();
    }

    Ctx.Restore();
  }
}

void FCairoCrosshairRenderer::RenderCircle(FCairoContext& Ctx, UUltiCrosshair *Crosshair) {}
void FCairoCrosshairRenderer::RenderNgon(FCairoContext& Ctx, UUltiCrosshair *Crosshair) {}
void FCairoCrosshairRenderer::RenderDot(FCairoContext& Ctx, UUltiCrosshair *Crosshair) {}
