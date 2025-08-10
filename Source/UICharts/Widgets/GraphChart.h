// Copyright Drop Games Inc.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "UICharts/Slate/SGraphChart.h"
#include "UICharts/Slate/Styles/RadarChartStyle.h"
#include "GraphChart.generated.h"



class SGraphChart;
/**
 * 
 */
UCLASS()
class UICHARTS_API UGraphChart : public UWidget
{
	GENERATED_BODY()
public:
	
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	FSlateBrush Brush;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	TSharedPtr<SGraphChart> SlateChart;

};
