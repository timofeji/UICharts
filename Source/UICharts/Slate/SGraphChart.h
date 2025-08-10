// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SGraphChart.generated.h"

USTRUCT(BlueprintType) //, meta = (HasNativeMake = ""))
struct FGraphStyleSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
	bool bShowAxes = true;
};

/**
 * 
 */
class UICHARTS_API SGraphChart : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGraphChart)
		{
		}

		SLATE_ATTRIBUTE(const FSlateBrush*, Brush)
	SLATE_END_ARGS()


	void SetBrush(FSlateBrush* InBrush);

	void SetData(TArray<FVector2D> InData);


	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	                      FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	                      bool bParentEnabled) const override;

protected:
	TArray<FVector2D> Data;
	FVector2D DataBounds;

private:
	FInvalidatableBrushAttribute Brush;
};
