// Copyright Drop Games Inc.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FRadarChartStyle;
/**
 * 

 */
class UICHARTS_API SRadarChart : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRadarChart)
		{
		}

		SLATE_ARGUMENT(const FRadarChartStyle*, ChartStyle)
	SLATE_END_ARGS()


	void      SetData(const TArray<float>& InData);
	void      SetData(const TArray<float>& InData, const TArray<FText>& InLabels);

	void SetChartStyle(FRadarChartStyle* InChartStyle);

	void Construct(const FArguments& InArgs);
	float EvaluateExpression(const FString& Expression);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;

protected:
	TArray<float>   Data;
	TArray<FText>   Labels;
private:
	const FRadarChartStyle* Style;
	float           AbsoluteDataBound;
};
