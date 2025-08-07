// Copyright Drop Games Inc.


#include "GraphChart.h"

#include "UICharts/Slate/SGraphChart.h"
#define LOCTEXT_NAMESPACE "UICharts"

void UGraphChart::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	SlateChart->SetBrush(&Brush);
}

void UGraphChart::ReleaseSlateResources(bool bReleaseChildren)
{
	SlateChart.Reset();
}

TSharedRef<SWidget> UGraphChart::RebuildWidget()
{
	SlateChart = SNew(SGraphChart)
		.Brush(&Brush);
	return SlateChart.ToSharedRef();
}

#if WITH_EDITOR
const FText UGraphChart::GetPaletteCategory()
{
	return LOCTEXT("UIChartsCategory", "UICharts");
}
#endif

#undef LOCTEXT_NAMESPACE
