// Copyright Drop Games Inc.


#include "RadarChartStyle.h"


FRadarChartStyle::FRadarChartStyle()
	:LabelBoxPadding(5.f),
	 LabelOffset(1.2f),
	 CapRadiusMultiplier(1.f)
{
}

FRadarChartStyle::~FRadarChartStyle()
{
}

const FName FRadarChartStyle::TypeName(TEXT("FRadarChartStyle"));

const FRadarChartStyle& FRadarChartStyle::GetDefault()
{
	static FRadarChartStyle Default;
	return Default;
}

void FRadarChartStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}
