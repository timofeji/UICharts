// Copyright Drop Games Inc.


#include "ColorPickerStyle.h"


FColorPickerStyle::FColorPickerStyle()
{
}

FColorPickerStyle::~FColorPickerStyle()
{
}

const FName FColorPickerStyle::TypeName(TEXT("FColorPickerStyle"));

const FColorPickerStyle& FColorPickerStyle::GetDefault()
{
	static FColorPickerStyle Default;
	return Default;
}

void FColorPickerStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}
