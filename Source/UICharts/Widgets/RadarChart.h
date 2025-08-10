// Copyright Drop Games Inc.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UICharts/Slate/Styles/RadarChartStyle.h"
#include "RadarChart.generated.h"

class SRadarChart;

/**
 * 
 */
UCLASS()
class UICHARTS_API URadarChart : public UWidget
{
	GENERATED_BODY()

public:
	
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	FRadarChartStyle ChartStyle;
	

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SRadarChart> SlateChart;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer")
	TArray<float> PreviewData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Designer")
	TArray<FText> PreviewDataLabels;
#endif
	
};
