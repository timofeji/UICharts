// Copyright Drop Games Inc.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "RadarChartStyle.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct UICHARTS_API FRadarChartStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FTextBlockStyle   TextBlockStyle;
	FRadarChartStyle& SetTextStyle( const FTextBlockStyle& InTextStyle ){ TextBlockStyle = InTextStyle ; return *this; }
	
	/** The brush used to draw an underline under (if any) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush BaseBrush;
	FRadarChartStyle& SetBaseBrush( const FSlateBrush& InBaseBrush ){ BaseBrush = InBaseBrush; return *this; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush DataBrush;
	FRadarChartStyle& SetDataBrush( const FSlateBrush& InDataBrush ){ DataBrush = InDataBrush; return *this; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush LabelBrush;
	FRadarChartStyle& SetLabelBrush( const FSlateBrush& InLabelBrush ){ LabelBrush = InLabelBrush; return *this; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush PointBrush;
	FRadarChartStyle& SetPointBrush( const FSlateBrush& InPointBrush ){ PointBrush = InPointBrush; return *this; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush PointCapBrush;
	FRadarChartStyle& SetPointCapBrush( const FSlateBrush& InPointCapBrush ){ PointCapBrush = InPointCapBrush; return *this; }
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	float LabelBoxPadding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	float LabelOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	float PointSizeMultiplier = 1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	float CapRadiusMultiplier = 1.f;
	

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	float               OutlineThickness = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FLinearColor              OutlineColor = FLinearColor::White;
	
	FRadarChartStyle();
	virtual ~FRadarChartStyle();
	
	
	// FSlateWidgetStyle
	virtual void                        GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName                  TypeName;
	virtual const FName                 GetTypeName() const override { return TypeName; };
	static const FRadarChartStyle& GetDefault();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class URadarChartStyleWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FRadarChartStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&WidgetStyle);
	}
};
