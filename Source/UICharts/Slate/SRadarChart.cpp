// Copyright Drop Games Inc.
#include "SRadarChart.h"
#include "SlateOptMacros.h"
#include "Fonts/FontMeasure.h"
#include "Styles/RadarChartStyle.h"

#define LOCTEXT_NAMESPACE "UICharts"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadarChart::Construct(const FArguments& InArgs)
{
	Style = InArgs._ChartStyle;

#if WITH_EDITOR
	const TArray<float> Mock       = {2.f, 5.f, 12.4f};
	const TArray<FText> MockLabels = {
		LOCTEXT("MockText", "Speed"), LOCTEXT("MockText", "Damage"), LOCTEXT("MockText", "Durability")
	};
	SetData(Mock, MockLabels);
#endif
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SRadarChart::SetData(const TArray<float>& InData)
{
	float MaxX = -FLT_MAX;

	//Preprocess
	for (const float X : InData)
	{
		if (X > MaxX)
		{
			MaxX = X;
		}
	}

	AbsoluteDataBound = MaxX;

	Data.Reset(InData.Num());
	for (int i = 0; i < InData.Num(); i++)
	{
		Data.Add(InData[i] / AbsoluteDataBound);
	}
}

void SRadarChart::SetData(const TArray<float>& InData, const TArray<FText>& InLabels)
{
	SetData(InData);
	Labels = InLabels;
}

int32 SRadarChart::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect&                        MyCullingRect,
	FSlateWindowElementList&                 OutDrawElements, int32 LayerId,
	const FWidgetStyle&                      InWidgetStyle,
	bool                                     bParentEnabled) const
{
	const int N = Data.Num();
	if (N < 3)
	{
		return LayerId;
	}

	const FSlateBrush* BaseBrush        = &Style->BaseBrush;
	const FSlateBrush* DataBrush        = &Style->DataBrush;
	const FSlateBrush* LabelBoxBrush    = &Style->LabelBrush;
	const FSlateBrush* PointBrush = &Style->PointBrush;
	const FSlateBrush* PointCapBrush = &Style->PointCapBrush;

	const FVector2D PointSize    = PointBrush->GetImageSize() * Style->PointSizeMultiplier;
	const FVector2D PointCapSize = PointCapBrush->GetImageSize();

	FLinearColor LinearColor = GetColorAndOpacity() * InWidgetStyle.GetColorAndOpacityTint();
	FColor DataColor =  (LinearColor* DataBrush->GetTint(InWidgetStyle)).ToFColor(true);
	FColor BaseColor =  (LinearColor* BaseBrush->GetTint(InWidgetStyle)).ToFColor(true);

	const FVector2D Pos    = AllottedGeometry.GetAbsolutePosition();
	const FVector2D Size   = AllottedGeometry.GetAbsoluteSize();
	const FVector2D Center = Pos + 0.5 * Size;

	FSlateVertex CenterVertex;
	CenterVertex.Position             = FVector2f(Center);
	CenterVertex.Color                = DataColor;
	CenterVertex.MaterialTexCoords[0] = 0.0f;
	CenterVertex.MaterialTexCoords[1] = 0.5f;
	CenterVertex.TexCoords[0]         = 0.0f;
	CenterVertex.TexCoords[1]         = 0.5f;
	CenterVertex.TexCoords[2]         = 0.0f;
	CenterVertex.TexCoords[3]         = 0.5f;

	TArray<FSlateVertex> BaseSurfaceVerts;
	TArray<FSlateVertex> DataSurfaceVerts;
	TArray<SlateIndex>   Indices;
	BaseSurfaceVerts.Add(CenterVertex);
	DataSurfaceVerts.Add(CenterVertex);
	DataSurfaceVerts[0].Color = DataColor;

	const float Radius = FMath::Min(Size.X, Size.Y) * 0.5f;
	const float ArcLen = (2 * PI) / N;

	int32 LabelLayerId = LayerId + 2;

	const FSlateRenderTransform& RenderTransform = AllottedGeometry.GetAccumulatedRenderTransform();
	for (int32 i = 0; i <= N; ++i)
	{
		// wrap
		const float     Angle    = (i < N ? i * ArcLen : 0.f) - (PI / 2);
		const FVector2D PointDir = FVector2D(FMath::Cos(Angle), FMath::Sin(Angle));
		const FVector2D PointCapLocation    = Radius * PointDir; // In radial coords
		const FVector2D PointLocation = Center + (Data[i < N ? i : 0]) * PointCapLocation;

		FSlateVertex    Vert;
		Vert.Position             = FVector2f(Center + PointCapLocation);
		Vert.Color                = BaseColor;
		Vert.MaterialTexCoords[0] = 1.f;
		Vert.MaterialTexCoords[1] = (i%2);
		Vert.TexCoords[0]         = 1.f;
		Vert.TexCoords[1]         = (i%2);
		Vert.TexCoords[2]         = 1.f;
		Vert.TexCoords[3]         = 1.0f;
		BaseSurfaceVerts.Add(Vert);

		FSlateVertex DataVert = Vert;
		DataVert.Position     = FVector2f(PointLocation);
		DataVert.Color        = DataColor;
		DataSurfaceVerts.Add(DataVert);

		////////////////////////////////////////////////////////////////////////////////////
		/// Draw Data Point/Cap
		////////////////////////////////////////////////////////////////////////////////////
		///
		const FVector2D PointBrushLocation = AllottedGeometry.AbsoluteToLocal(PointLocation);
		FSlateDrawElement::MakeRotatedBox(
			OutDrawElements,
			LabelLayerId + 2,
			AllottedGeometry.ToPaintGeometry(PointSize,
				FSlateLayoutTransform(PointBrushLocation - PointSize * .5f)),
			PointBrush,
			ESlateDrawEffect::None,
			Angle,
			PointSize * .5f,
			FSlateDrawElement::RelativeToElement,
			PointBrush->GetTint(InWidgetStyle)
			);
		 
		const FVector2D PointCapBrushLocation = AllottedGeometry.AbsoluteToLocal(Center + PointCapLocation * Style->CapRadiusMultiplier);
		FSlateDrawElement::MakeRotatedBox(
			OutDrawElements,
			LabelLayerId + 2,
			AllottedGeometry.ToPaintGeometry(PointCapSize,
				FSlateLayoutTransform(PointCapBrushLocation - PointCapSize * .5f)),
			PointCapBrush,
			ESlateDrawEffect::None,
			Angle,
			PointCapSize * .5f,
			FSlateDrawElement::RelativeToElement,
			PointCapBrush->GetTint(InWidgetStyle)
			);
	}

	//close loop
	for (int32 i = 1; i <= N; ++i)
	{
		Indices.Add(0);
		Indices.Add(i);
		Indices.Add(i + 1);
	}

	// Draw Base Polygon
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		LayerId++,
		BaseBrush->GetRenderingResource(),
		BaseSurfaceVerts,
		Indices,
		nullptr,
		0,
		0,
		ESlateDrawEffect::None
		);
	
	// Draw Data Surface
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		LayerId++,
		DataBrush->GetRenderingResource(),
		DataSurfaceVerts,
		Indices,
		nullptr,
		0,
		0,
		ESlateDrawEffect::None
		);
	
	
	if (Style->OutlineThickness > 0.f)
	{

		TArray<FVector2D> BaseSurfaceLineSegments;
		TArray<FVector2D> DataSurfaceLineSegments;
		DataSurfaceLineSegments.Reserve(DataSurfaceVerts.Num() - 1);
		BaseSurfaceLineSegments.Reserve(BaseSurfaceVerts.Num() - 1);

		for (int32 i = 1; i < BaseSurfaceVerts.Num() ; i++)
		{
			BaseSurfaceLineSegments.Add(AllottedGeometry.AbsoluteToLocal(BaseSurfaceVerts[i].Position));
			DataSurfaceLineSegments.Add(AllottedGeometry.AbsoluteToLocal(DataSurfaceVerts[i].Position));
		}

		// BaseSurfaceLineSegments.Add(AllottedGeometry.AbsoluteToLocal(Center + PointDir));

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId++,
			AllottedGeometry.ToPaintGeometry(),
			BaseSurfaceLineSegments,
			ESlateDrawEffect::None,
			Style->OutlineColor,    // Line color
			true,                   // Anti-alias
			Style->OutlineThickness // Line thickness
			);
		
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId++,
			AllottedGeometry.ToPaintGeometry(),
			DataSurfaceLineSegments,
			ESlateDrawEffect::None,
			Style->OutlineColor,          // Line color
			true,                         // Anti-alias
			Style->OutlineThickness * .5f // Line thickness
			);
		
	
	}

	for (int32 i = 0; i < N; ++i)
	{
		if (Labels.IsValidIndex(i))
		{
			const FSlateFontInfo& TextFont    = Style->TextBlockStyle.Font;
			const FText&          LabelString = Labels[i];

			const TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->
																							  GetFontMeasureService();
			FVector2D LabelSize = FontMeasureService->Measure(LabelString, TextFont);

			const float     Angle    = (i < N ? i * ArcLen : 0.f) - (PI / 2);
			const FVector2D PointDir = FVector2D(FMath::Cos(Angle), FMath::Sin(Angle));
			const FVector2D Point    = Radius * PointDir; // In radial coords


			const FVector2D LabelPosition = AllottedGeometry.AbsoluteToLocal(
												Center + Point * Style->LabelOffset) - (LabelSize * .5f);
			const FVector2D BoxPadding = FVector2D(Style->LabelBoxPadding);

			////////////////////////////////////////////////////////////////////////////////////
			/// Draw Data Label
			////////////////////////////////////////////////////////////////////////////////////
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LabelLayerId,
				AllottedGeometry.ToPaintGeometry(LabelSize + 2.0f * BoxPadding,
					FSlateLayoutTransform(LabelPosition - BoxPadding)),
				LabelBoxBrush,
				ESlateDrawEffect::None,
				LabelBoxBrush->GetTint(InWidgetStyle)
				);

			FPaintGeometry TextGeometry = AllottedGeometry.ToPaintGeometry(
				FSlateLayoutTransform(LabelPosition)
				);
			FSlateDrawElement::MakeText(
				OutDrawElements,
				LabelLayerId + 2,
				TextGeometry,
				LabelString,
				TextFont,
				ESlateDrawEffect::None,
				Style->TextBlockStyle.ColorAndOpacity.GetColor(InWidgetStyle)
				);
			////////////////////////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////////////
		}
	}


	return LayerId;
}



void SRadarChart::SetChartStyle(FRadarChartStyle* InChartStyle)
{
	// Brush.SetImage(*this, InBrush);
}
#undef LOCTEXT_NAMESPACE