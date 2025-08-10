// Copyright Drop Games Inc.


#include "RadarChart.h"

#include "UICharts/Slate/SRadarChart.h"
#define LOCTEXT_NAMESPACE "UICharts"

void URadarChart::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	SlateChart->SetChartStyle(&ChartStyle);
	
#if WITH_EDITORONLY_DATA
	if (PreviewData.Num() > 0)
	{
		SlateChart->SetData(PreviewData, PreviewDataLabels);
	}
#endif
}

void URadarChart::ReleaseSlateResources(bool bReleaseChildren)
{
	SlateChart.Reset();
}

TSharedRef<SWidget> URadarChart::RebuildWidget()
{
	SlateChart = SNew(SRadarChart)
		.ChartStyle(&ChartStyle);
	return SlateChart.ToSharedRef();
}

#if WITH_EDITOR
const FText URadarChart::GetPaletteCategory()
{
	return LOCTEXT("UIChartsCategory", "UICharts");
}
#endif

#undef LOCTEXT_NAMESPACE
