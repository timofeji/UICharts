// Copyright Drop Games Inc.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "ColorPickerStyle.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct UICHARTS_API FColorPickerStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()
	
	/** The brush used to draw an underline under (if any) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush RectangleBrush;
	FColorPickerStyle & SetRectangleBrush( const FSlateBrush& InRectangleBrush ){ RectangleBrush = InRectangleBrush; return *this; }
	
	/** The brush used to draw an underline under (if any) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush SliderHandleBrush;
	FColorPickerStyle & SetSliderHandleBrush( const FSlateBrush& InSliderHandleBrush ){ SliderHandleBrush = InSliderHandleBrush; return *this; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush HueBrush;
	FColorPickerStyle & SetHueBrush( const FSlateBrush& InHueBrush ){ HueBrush = InHueBrush; return *this; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Appearance, AdvancedDisplay)
	FSlateBrush CursorBrush;
	FColorPickerStyle & SetCursorBrush( const FSlateBrush& InCursorBrush ){ CursorBrush = InCursorBrush; return *this; }
	

	// FSlateWidgetStyle
	virtual void                         GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName                   TypeName;
	virtual const FName                  GetTypeName() const override { return TypeName; };
	static const FColorPickerStyle& GetDefault();

	FColorPickerStyle();
	virtual ~FColorPickerStyle();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UColorPickerStyleWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FColorPickerStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&WidgetStyle);
	}
};
