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
	const TArray<float> Mock = {2.f, 5.f, 12.4f};
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
	Data = InData;
}


void SRadarChart::SetData(const TArray<float>& InData, const TArray<FText>& InLabels)
{
	SetData(InData);
	Labels = InLabels;
}


int32 SRadarChart::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                           const FSlateRect& MyCullingRect,
                           FSlateWindowElementList& OutDrawElements, int32 LayerId,
                           const FWidgetStyle& InWidgetStyle,
                           bool bParentEnabled) const
{
	const int N = Data.Num();
	if (N < 3)
	{
		return LayerId;
	}


	TArray<FSlateVertex> Vertices;
	TArray<SlateIndex> Indices;
	
	TArray<FVector2D> FullLineSegments;
	TArray<FVector2D> PartLineSegments;

	const FSlateBrush* SlateBrush = &Style->BaseBrush;
	const FSlateBrush* LabelBoxBrush = &Style->LabelBrush;
	const FSlateBrush* PointCapBoxBrush = &Style->PointCapBrush;

	const FVector2D PointCapSize = PointCapBoxBrush->GetImageSize();
	
	

	FSlateResourceHandle Handle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*SlateBrush);
	FLinearColor LinearColor = GetColorAndOpacity() * InWidgetStyle.GetColorAndOpacityTint() * SlateBrush->
		GetTint(InWidgetStyle);
	FColor FinalColorAndOpacity = LinearColor.ToFColor(true);

	const FVector2D Pos = AllottedGeometry.GetAbsolutePosition();
	const FVector2D Size = AllottedGeometry.GetAbsoluteSize();
	const FVector2D Center = Pos + 0.5 * Size;

	Vertices.AddZeroed();
	FSlateVertex& CenterVertex = Vertices.Last();
	CenterVertex.Position = FVector2f(Center);
	CenterVertex.Color = FinalColorAndOpacity;
	CenterVertex.MaterialTexCoords[0] = 0.f;
	CenterVertex.MaterialTexCoords[1] = 0.5f;
	CenterVertex.TexCoords[0] = 0.f;
	CenterVertex.TexCoords[1] = 0.5f;
	CenterVertex.TexCoords[2] = 0.f;
	CenterVertex.TexCoords[3] = 0.5f;

	const float Radius   = FMath::Min(Size.X, Size.Y) * 0.5f;
	const float ArcLen   = (2 * PI) / N;

	

	for (int32 i = 0; i <= N; ++i)
	{
		// wrap
		const float Angle = (i < N ? i * ArcLen : 0.f) - (PI / 2);
		const FVector2D EdgeDirection = Radius * FVector2D(
			FMath::Cos(Angle), FMath::Sin(Angle));

		PartLineSegments.Add(
			AllottedGeometry.AbsoluteToLocal(Center + (Data[i < N ? i : 0] / AbsoluteDataBound) * EdgeDirection));
		FullLineSegments.Add(AllottedGeometry.AbsoluteToLocal(Center + EdgeDirection));

		const FVector2D UV_Data{0.5f, (i % 2) * 1.f};


		FSlateVertex V0;
		V0.Position = FVector2f(Center + (Data[i < N ? i : 0] / AbsoluteDataBound) * EdgeDirection);
		V0.Color = FinalColorAndOpacity;
		V0.MaterialTexCoords[0] = UV_Data.X;
		V0.MaterialTexCoords[1] = UV_Data.Y;
		V0.TexCoords[0] = UV_Data.X;
		V0.TexCoords[1] = UV_Data.Y;
		V0.TexCoords[2] = UV_Data.X;
		V0.TexCoords[3] = UV_Data.Y;
		Vertices.Add(V0);

		if (Labels.IsValidIndex(i))
		{
			const FSlateFontInfo& TextFont    = Style->TextBlockStyle.Font;
			const FText&          LabelString = Labels[i];

			const TSharedRef<FSlateFontMeasure> FontMeasureService = FSlateApplication::Get().GetRenderer()->
				GetFontMeasureService();
			FVector2D LabelSize = FontMeasureService->Measure(LabelString, TextFont);

			const FVector2D PointCapLocation = AllottedGeometry.AbsoluteToLocal(Center + EdgeDirection  * Style->CapRadiusMultiplier );
			const FVector2D LabelPosition = AllottedGeometry.AbsoluteToLocal(
				Center + EdgeDirection * Style->LabelRadiusMultiplier) - (LabelSize * .5f);
			const FVector2D BoxPadding    = FVector2D(Style->LabelBoxPadding);

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId + 1,
				AllottedGeometry.ToPaintGeometry(LabelSize + 2.0f * BoxPadding ,
				                                 FSlateLayoutTransform(LabelPosition - BoxPadding)),
				LabelBoxBrush ,
				ESlateDrawEffect::None,
				LabelBoxBrush->GetTint(InWidgetStyle)
			);


			FSlateDrawElement::MakeRotatedBox(
				OutDrawElements,
				LayerId + 2,
				AllottedGeometry.ToPaintGeometry(PointCapSize,
				                                 FSlateLayoutTransform(
					                                 PointCapLocation - PointCapSize * .5f)),

				PointCapBoxBrush,
				ESlateDrawEffect::None,
				Angle,
 PointCapSize*.5f,
				FSlateDrawElement::RelativeToElement,
				PointCapBoxBrush->GetTint(InWidgetStyle)
			);

			FPaintGeometry TextGeometry = AllottedGeometry.ToPaintGeometry(
				FSlateLayoutTransform(LabelPosition )
			);
			FSlateDrawElement::MakeText(
				OutDrawElements,
				LayerId + 6,
				TextGeometry,
				LabelString,
				TextFont,
				ESlateDrawEffect::None,
				FinalColorAndOpacity
			);
		}
	}

	//close loop
	for (int32 i = 1; i <= N; ++i)
	{
		Indices.Add(0);
		Indices.Add(i);
		Indices.Add(i + 1);
	}

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId + 2,
		AllottedGeometry.ToPaintGeometry(),
		FullLineSegments,
		ESlateDrawEffect::None,
		FLinearColor::White, // Line color
		true, // Anti-alias
		6.0f // Line thickness
	);

	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId + 2,
		AllottedGeometry.ToPaintGeometry(),
		PartLineSegments,
		ESlateDrawEffect::None,
		FLinearColor::White, // Line color
		true, // Anti-alias
		2.5f // Line thickness
	);

	// Use MakeCustomVerts to draw the polygon and apply a material
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		LayerId,
		Handle,
		Vertices,
		Indices,
		nullptr, // No clipping
		0, // Layer
		0 // Render effects
	);

	// Use MakeCustomVerts to draw the polygon and apply a material
	FSlateDrawElement::MakeCustomVerts(
		OutDrawElements,
		LayerId,
		Handle,
		Vertices,
		Indices,
		nullptr, // No clipping
		0, // Layer
		0 // Render effects
	);
	

	return LayerId;
}

void SRadarChart::SetChartStyle(FRadarChartStyle * InChartStyle)
{
	// Brush.SetImage(*this, InBrush);
}
#undef LOCTEXT_NAMESPACE
