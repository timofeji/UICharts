// Fill out your copyright notice in the Description page of Project Settings.
#include "SGraphChart.h"
#include "SlateOptMacros.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SGraphChart::SetData(TArray<FVector2D> InData)
{
	float MaxX = -FLT_MAX;
	float MaxY = -FLT_MAX;
	float MinX = FLT_MAX;
	float MinY = FLT_MAX;

	//Preprocess
	for (const FVector2D& Vec : InData)
	{
		if (Vec.X > MaxX)
		{
			MaxX = Vec.X;
		}
		if (Vec.X < MinX)
		{
			MinX = Vec.X;
		}

		if (Vec.Y > MaxY)
		{
			MaxY = Vec.Y;
		}
		
		if (Vec.Y < MinY)
		{
			MinY = Vec.Y;
		}
	}

	DataBounds = FVector2D(1 / (MaxX + FMath::Abs(MinX)), 1 / (MaxY + FMath::Abs(MinY)));

	Data = InData;
}

void SGraphChart::Construct(const FArguments& InArgs)
{
	Brush = FInvalidatableBrushAttribute(InArgs._Brush);

}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

//
// float SGraphChart::EvaluateExpression(const FString& Expression)
// {
// 	// Map operators to their respective functions
// 	TMap<FString, TFunction<float(float, float)>> OperatorMap;
// 	OperatorMap.Add(TEXT("+"), [](float A, float B) { return A + B; });
// 	OperatorMap.Add(TEXT("-"), [](float A, float B) { return A - B; });
// 	OperatorMap.Add(TEXT("*"), [](float A, float B) { return A * B; });
// 	OperatorMap.Add(TEXT("/"), [](float A, float B) { return B != 0.0f ? A / B : 0.0f; }); // Handle divide by zero
//
// 	TArray<FString> ParsedTokens;
// 	Expression.ParseIntoArray(ParsedTokens, TEXT(" "), true); // Split by spaces
//
// 	if (ParsedTokens.Num() < 3)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Invalid expression format"));
// 		return 0.0f;
// 	}
//
// 	// Initial value
// 	float Result = FCString::Atof(*ParsedTokens[0]);
//
// 	// Traverse through the expression
// 	for (int32 i = 1; i < ParsedTokens.Num(); i += 2)
// 	{
// 		FString Operator = ParsedTokens[i];
// 		float Operand = FCString::Atof(*ParsedTokens[i + 1]);
//
// 		if (OperatorMap.Contains(Operator))
// 		{
// 			Result = OperatorMap[Operator](Result, Operand);
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Error, TEXT("Unknown operator: %s"), *Operator);
// 			return 0.0f;
// 		}
// 	}
//
// 	return Result;
// }

int32 SGraphChart::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                           const FSlateRect& MyCullingRect,
                           FSlateWindowElementList& OutDrawElements, int32 LayerId,
                           const FWidgetStyle& InWidgetStyle,
                           bool bParentEnabled) const
{
	if (Data.Num() > 1)
	{
		TArray<FSlateVertex> Vertices;
		TArray<SlateIndex> Indices;
		TArray<FVector2D> LineSegments;

		const FSlateBrush* SlateBrush = Brush.GetImage().Get();
		FSlateResourceHandle Handle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*SlateBrush);
		FLinearColor LinearColor = GetColorAndOpacity() * InWidgetStyle.GetColorAndOpacityTint() * SlateBrush->
			GetTint(InWidgetStyle);
		FColor FinalColorAndOpacity = LinearColor.ToFColor(true);

		// Get the size of the widget
		FVector2D Size = AllottedGeometry.GetLocalSize();
		//
		//
		// Create a polygon from the time series data


		const double UV_X_Size = 1.f / Data.Num();
		for (int32 i = 0; i < Data.Num(); ++i)
		{
			FVector2D Point = Data[i] * DataBounds;
			Point.Y = Point.Y  + 0.5f; // Offset into visible range
			Point.X = Point.X  + 0.5f; // Offset into visible range

			LineSegments.Add(AllottedGeometry.GetLocalPositionAtCoordinates(Point));

			const double UV_X = i * (UV_X_Size);
			const double UV_Y = Point.Y;


			const FVector2D UV_Data = FVector2D(UV_X, UV_Y);
			const FVector2D UV_Root = FVector2D(UV_X, 0.5f);


			FSlateVertex DataVert;
			DataVert.Position = FVector2f(AllottedGeometry.GetAbsolutePositionAtCoordinates(Point));
			DataVert.Color = FinalColorAndOpacity;
			DataVert.MaterialTexCoords[0] = UV_Data.X;
			DataVert.MaterialTexCoords[1] = UV_Data.Y;
			DataVert.TexCoords[0] = UV_Data.X;
			DataVert.TexCoords[1] = UV_Data.Y;
			DataVert.TexCoords[2] = UV_Data.X;
			DataVert.TexCoords[3] = UV_Data.Y;


			Point.Y = 0.5f;
			FSlateVertex RootVert;
			RootVert.Position = FVector2f(AllottedGeometry.GetAbsolutePositionAtCoordinates(Point));
			RootVert.Color = FinalColorAndOpacity;
			RootVert.MaterialTexCoords[0] = UV_Root.X;
			RootVert.MaterialTexCoords[1] = UV_Root.Y;
			RootVert.TexCoords[0] = UV_Root.X;
			RootVert.TexCoords[1] = UV_Root.Y;
			RootVert.TexCoords[2] = UV_Root.X;
			RootVert.TexCoords[3] = UV_Root.Y;


			Vertices.Add(RootVert);
			Vertices.Add(DataVert);

			// Add indices for drawing the polygon as a triangle strip
			if (i > 0)
			{
				int32 index = i * 2;

				Indices.Add(index - 2);
				Indices.Add(index - 1);
				Indices.Add(index);

				Indices.Add(index + 1);
				Indices.Add(index);
				Indices.Add(index - 1);
			}
		}


		// Use MakeCustomVerts to draw the polygon and apply a material
		FSlateDrawElement::MakeCustomVerts(
			OutDrawElements,
			LayerId,
			Handle,
			Vertices,
			Indices,
			nullptr, // No clipping
			LayerId++, // Layer
			0 // Render effects
		);


		TArray<FVector2D> XAxis = {
			AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(0.f, .5f)),
			AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(1.f, .5f)),

		};
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId++,
			AllottedGeometry.ToPaintGeometry(),
			XAxis,
			ESlateDrawEffect::None,
			FLinearColor::Black, // Line color
			true, // Anti-alias
			1.0f // Line thickness
		);

		TArray<FVector2D> YAxis = {
			AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(.5f, 0.f)),
			AllottedGeometry.GetLocalPositionAtCoordinates(FVector2D(.5f, 1.f))
		};
		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId++,
			AllottedGeometry.ToPaintGeometry(),
			YAxis,
			ESlateDrawEffect::None,
			FLinearColor::Black, // Line color
			true, // Anti-alias
			1.0f // Line thickness
		);


		// for (int32 Index = 0; Index < Data.Num() - 1; ++Index)
		// {
		// 	FVector2D Point = Data[Index] * DataBounds;
		// 	FVector2D PointA = AllottedGeometry.GetLocalPositionAtCoordinates(Point);
		// 	LineSegments.Add(PointA);
		// }

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId + 1,
			AllottedGeometry.ToPaintGeometry(),
			LineSegments,
			ESlateDrawEffect::None,
			FLinearColor::White, // Line color
			true, // Anti-alias
			3.0f // Line thickness
		);
	}

	return LayerId;
}

void SGraphChart::SetBrush(FSlateBrush* InBrush)
{
	Brush.SetImage(*this, InBrush);
}
