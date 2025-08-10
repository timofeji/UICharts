// Copyright Drop Games Inc.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

struct FColorPickerStyle;
/**
 * 
 */
class SBorder;

DECLARE_DELEGATE_OneParam(FOnColorValueChanged, FLinearColor);

class UICHARTS_API SGameColorPicker : public SCompoundWidget
{
	enum class EColorChannel : uint8
	{
		R,
		G,
		B,
		A,
		H,
		S,
		V,
		HEX
	};

public:
	SLATE_BEGIN_ARGS(SGameColorPicker)
		{
		}

		SLATE_ATTRIBUTE(FLinearColor, InitialColor)
		SLATE_EVENT(FOnColorValueChanged, OnColorChanged)
		SLATE_STYLE_ARGUMENT(FColorPickerStyle, ColorPickerStyle)
		
	SLATE_END_ARGS()

	FText GetColorHexString() const;
	void Construct(const FArguments& InArgs);
	~SGameColorPicker();
	 FReply OnPickerMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) ;
	 FReply OnPickerMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) ;
	 FReply OnPickerMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) ;
	

	void InitFromColor(const FLinearColor& Attribute);
	
	void UpdateColorFromHSV();

	void OnHexStringCommitted(const FText& Text, ETextCommit::Type Arg);


	FLinearColor RGB;
	FLinearColor HSV;
	

private:
		
	float Hue;
	float Saturation;
	float Value;
	float Alpha = 1.f;
	
	
	FOnColorValueChanged OnColorChanged;
	
	FSlateBrush DynamicBrush;
	UMaterialInstanceDynamic* DynamicMaterial;
	const FColorPickerStyle* Style;
	TSharedRef<SWidget> MakeColorSpinBox(EColorChannel Channel,FLinearColor DisplayColor);
};