#include "../UltiCrossPCH.h"

#include "UltiCrosshair.h"

#include "CairoCrosshairRenderer.h"

FCairoRenderContext::FCairoRenderContext(UTexture2D* Tex)
  : FRenderContext(Tex)
  , CairoSurface(nullptr)
  , CairoContext(nullptr)
{
}

void FCairoRenderContext::Begin()
{
  FRenderContext::Begin();

  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, SizeX());
  CairoSurface = cairo_image_surface_create_for_data(RawData, CAIRO_FORMAT_ARGB32, SizeX(), SizeY(), stride);
  CairoContext = cairo_create(CairoSurface);
}

void FCairoRenderContext::End()
{
  cairo_destroy(CairoContext);
  CairoContext = nullptr;

  cairo_surface_destroy(CairoSurface);
  CairoSurface = nullptr;

  FRenderContext::End();
}

FCairoSession::FCairoSession(FCairoRenderContext *RenderContext)
  : Ctx(RenderContext)
{
  cairo_save(Ctx->CairoContext);
}

FCairoSession::~FCairoSession()
{
  cairo_restore(Ctx->CairoContext);
}

void FCairoSession::Clear()
{
  cairo_set_source_rgba(Ctx->CairoContext, 0, 0, 0, 0);
  cairo_set_operator(Ctx->CairoContext, CAIRO_OPERATOR_SOURCE);
  cairo_paint(Ctx->CairoContext);
}

void FCairoSession::MoveTo(const FVector2D& Point)
{
  cairo_move_to(Ctx->CairoContext, Point.X, Point.Y);
}

void FCairoSession::LineTo(const FVector2D& Point)
{
  cairo_line_to(Ctx->CairoContext, Point.X, Point.Y);
}

void FCairoSession::RelativeLineTo(const FVector2D& Vec)
{
  cairo_rel_line_to(Ctx->CairoContext, Vec.X, Vec.Y);
}

void FCairoSession::ClosePath()
{
  cairo_close_path(Ctx->CairoContext);
}

void FCairoSession::Translate(const FVector2D& V)
{
  cairo_translate(Ctx->CairoContext, V.X, V.Y);
}

void FCairoSession::Rotate(float Angle)
{
  cairo_rotate(Ctx->CairoContext, Angle);
}

void FCairoSession::RotateAround(const FVector2D& P, float Angle)
{
  Translate(P);
  Rotate(Angle);
  Translate(-1 * P);
}

void FCairoSession::SetLineWidth(float Width)
{
  cairo_set_line_width(Ctx->CairoContext, Width);
}

void FCairoSession::SetOperator(ECairoOperator Op)
{
  cairo_set_operator(Ctx->CairoContext, (cairo_operator_t)Op);
}

void FCairoSession::Fill(const FLinearColor& Color, bool bPreserve)
{
  cairo_set_source_rgba(Ctx->CairoContext, Color.R, Color.G, Color.B, Color.A);

  if (bPreserve)
  {
    cairo_fill_preserve(Ctx->CairoContext);
  }
  else
  {
    cairo_fill(Ctx->CairoContext);
  }
}

void FCairoSession::Stroke(const FLinearColor& Color, bool bPreserve)
{
  cairo_set_source_rgba(Ctx->CairoContext, Color.R, Color.G, Color.B, Color.A);

  if (bPreserve)
  {
    cairo_stroke_preserve(Ctx->CairoContext);
  }
  else
  {
    cairo_stroke(Ctx->CairoContext);
  }
}

FCairoCrosshairRenderer::FCairoCrosshairRenderer()
{
}

void FCairoCrosshairRenderer::Render(UUltiCrosshair *Crosshair)
{
  ensure(Crosshair);

  FCairoRenderContext Ctx(Crosshair->GetTexture());

  Ctx.Begin();
  RenderBackground(&Ctx);
  RenderCrosshairs(&Ctx, Crosshair);
  RenderCircle(&Ctx, Crosshair);
  RenderNgon(&Ctx, Crosshair);
  RenderDot(&Ctx, Crosshair);
  Ctx.End();
}

void FCairoCrosshairRenderer::RenderBackground(FCairoRenderContext *Ctx)
{
  FCairoSession S(Ctx);
  S.Clear();
}

void FCairoCrosshairRenderer::RenderCrosshairs(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair)
{
  if (Crosshair->Type != EUltiCrossCrosshairType::Crosshairs) return;

  bool bOffsetByOne = false;

  float Thickness = Crosshair->Thickness;
  float Outline = Crosshair->Outline;
  float Rotation = FMath::Clamp(Crosshair->Rotation, 0.0f, 360.0f);

  FUltiCrossCrosshairParams Params(Crosshair->Crosshairs);

  // TODO: Remove these from UUltiCrosshair
  Params.CenterGap = Crosshair->Gap;
  Params.Length = Crosshair->Length;

  FVector2D Center(Ctx->SizeX() / 2.0f, Ctx->SizeY() / 2.0f);

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
    FCairoSession S(Ctx);

    // Rotate about the center
    S.Translate(Center);
    S.Rotate(FMath::DegreesToRadians(Angle));

    FVector2D OutlineVec(0, Outline);
    FVector2D StartVec(0, -1 * Params.CenterGap);
    FVector2D LengthVec(0, Params.Length);

    // Draw the outline stroke first
    S.MoveTo(StartVec + OutlineVec);
    S.RelativeLineTo((-1 * LengthVec) - (2 * OutlineVec));
    S.SetLineWidth(2.0f * (Outline + Thickness));
    S.Stroke(FLinearColor::Black);

    // Draw the fill
    S.MoveTo(StartVec);
    S.RelativeLineTo(-1 * LengthVec);
    S.SetLineWidth(2.0f * Thickness);
    S.SetOperator(ECairoOperator::Source);
    S.Stroke(FLinearColor::White);
  }
}

void FCairoCrosshairRenderer::RenderCircle(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair) {}
void FCairoCrosshairRenderer::RenderNgon(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair) {}
void FCairoCrosshairRenderer::RenderDot(FCairoRenderContext *Ctx, UUltiCrosshair *Crosshair) {}