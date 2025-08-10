// Copyright Drop Games Inc.


#include "SGameColorPicker.h"
#include "InputCoreTypes.h"

#include "SlateOptMacros.h"
#include "Styles/ColorPickerStyle.h"
#include "Widgets/Input/SSpinBox.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION


class SColorPickerSlider : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SColorPickerSlider)
		{
		}
	SLATE_ARGUMENT(const FSlateBrush*, HandleBrush)
		
	SLATE_STYLE_ARGUMENT(FColorPickerStyle, Style)
		
	SLATE_ARGUMENT(TFunction<void(float)>, OnValueChanged) // Set binding
	SLATE_ATTRIBUTE(float, Value)
		
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs)
	{
		Style = InArgs._Style;
		OnValueChanged = InArgs._OnValueChanged;
		ValueAttribute = InArgs._Value;

		const FSlateBrush* BackgroundBrush = &Style->HueBrush;
		ChildSlot
		[
			SNew(SImage)
			.Image(BackgroundBrush)
		];
	}
	
private:
	bool Dragging = false;
	TFunction<void(float)> OnValueChanged;
	TAttribute<float> ValueAttribute;

	const FColorPickerStyle* Style;
	

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override
	{
		LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);


		if (Style && Style->SliderHandleBrush.IsSet())
		{
			const FVector2D HandleSize = Style->SliderHandleBrush.GetImageSize();
			const FVector2D HandleHalfSize = HandleSize*.5f;

			const float AllottedHeight = AllottedGeometry.Size.Y;
			const float AllottedWidth = AllottedGeometry.Size.X;

			const FVector2D Offset =
				FVector2D(-10,
				          ((AllottedHeight * ValueAttribute.Get()) - HandleHalfSize.Y));
			
			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId + 10,
				AllottedGeometry.ToPaintGeometry(
					HandleSize + FVector2D(20, 0),
					FSlateLayoutTransform(Offset)
				),
				&Style->SliderHandleBrush,
				ESlateDrawEffect::None,
				InWidgetStyle.GetColorAndOpacityTint());
		}

		return LayerId;
	}
	
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			Dragging = true;
			UpdateValue(MyGeometry, MouseEvent);
			
			return FReply::Handled().CaptureMouse(SharedThis(this));
		}
		return FReply::Unhandled();
	}

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (Dragging && HasMouseCapture())
		{
			UpdateValue(MyGeometry, MouseEvent);
			return FReply::Handled();
		}
		return FReply::Unhandled();
	}

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (Dragging && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			Dragging = false;
			
			return FReply::Handled().ReleaseMouseCapture();
		}
		return FReply::Unhandled();
	}
	
	void UpdateValue(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
	{
		FVector2D LocalPos = Geometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		FVector2D Size = Geometry.GetLocalSize();

		LocalPos.Y = FMath::Clamp(LocalPos.Y, 0.0f, Size.Y);

		float V = (LocalPos.Y / Size.Y);
		
		if (OnValueChanged.IsSet())
		{
			OnValueChanged(V);
		}
	}
};

class SColorPickerRectangle : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SColorPickerRectangle)
		{
		}

	SLATE_STYLE_ARGUMENT(FColorPickerStyle, Style)
	SLATE_ARGUMENT(TFunction<void(FVector2D)>, OnSVChanged) 
	SLATE_ATTRIBUTE(FLinearColor, HSV)
		
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs)
	{
		HSVAttribute = InArgs._HSV;
		OnSVChanged  = InArgs._OnSVChanged;
		Style        = InArgs._Style;

		Dragging = false;
		ChildSlot
		[
			SAssignNew(RectangleImage, SImage)
			.Image(&Style->RectangleBrush)
			.DesiredSizeOverride(FVector2D(255))
		];
	}

private:
	bool Dragging;
	TAttribute<FLinearColor> HSVAttribute;
	TFunction<void(FVector2D)> OnSVChanged;
	TSharedPtr<SImage> RectangleImage;
	
	const FColorPickerStyle* Style;

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override
	{
		LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);


		OutDrawElements.PushClip(FSlateClippingZone(AllottedGeometry.GetLayoutBoundingRect()));
		if (Style && Style->CursorBrush.IsSet())
		{

			const FVector2D HandleHalfSize = Style->CursorBrush.GetImageSize() *.5;
			const float AllottedHeight = AllottedGeometry.Size.Y;
			const float AllottedWidth = AllottedGeometry.Size.Y;

			const FLinearColor HSV    = HSVAttribute.Get();
			const FVector2D    Offset = FVector2D((AllottedWidth * HSV.G) - HandleHalfSize.X,
			                                   (AllottedHeight * (1 - HSV.B)) - HandleHalfSize.Y);

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId + 1,
				AllottedGeometry.ToPaintGeometry(
					Style->CursorBrush.GetImageSize(),
					FSlateLayoutTransform(FVector2D(Offset))
				),
				&Style->CursorBrush,
				ESlateDrawEffect::None,
				InWidgetStyle.GetColorAndOpacityTint());
		}

		OutDrawElements.PopClip();
		
		return LayerId;
	}

	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			Dragging = true;
			UpdateSVFromMouse(MyGeometry, MouseEvent);
			
			return FReply::Handled().CaptureMouse(SharedThis(this));
		}
		return FReply::Unhandled();
	}

	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (Dragging && HasMouseCapture())
		{
			UpdateSVFromMouse(MyGeometry, MouseEvent);
			return FReply::Handled();
		}
		return FReply::Unhandled();
	}

	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override
	{
		if (Dragging && MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			Dragging = false;
			
			return FReply::Handled().ReleaseMouseCapture();
		}
		return FReply::Unhandled();
	}
	
	void UpdateSVFromMouse(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
	{
		FVector2D LocalPos = Geometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		FVector2D Size = Geometry.GetLocalSize();

		// Clamp to widget bounds
		LocalPos.X = FMath::Clamp(LocalPos.X, 0.0f, Size.X);
		LocalPos.Y = FMath::Clamp(LocalPos.Y, 0.0f, Size.Y);

		// Normalize to [0, 1]
		float S = LocalPos.X / Size.X;
		float V = 1.0f - (LocalPos.Y / Size.Y); // Flip Y

		if (OnSVChanged.IsSet())
		{
			OnSVChanged(FVector2D(S, V));
		}


	}
};

void SGameColorPicker::InitFromColor(const FLinearColor& Color)
{
	RGB = Color;
	HSV = Color.LinearRGBToHSV();

	Hue        = HSV.R/360;
	Saturation = HSV.G;
	Value      = HSV.B;

	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("Hue"), Hue);
	}
}

FText SGameColorPicker::GetColorHexString() const
{
	FColor SRGBColor = RGB.ToFColorSRGB(); // Convert to sRGB (clamped and gamma corrected)
	return FText::FromString(FString::Printf(TEXT("#%02X%02X%02X%02X"),
						   SRGBColor.R,
						   SRGBColor.G,
						   SRGBColor.B,
						   SRGBColor.A));
}

void SGameColorPicker::Construct(const FArguments& InArgs)
{
	OnColorChanged = InArgs._OnColorChanged;
	Style = InArgs._ColorPickerStyle;

	UObject*            Resource = Style->RectangleBrush.GetResourceObject();
	UMaterialInterface* Material = Cast<UMaterialInterface>(Resource);
	if (Material)
	{
		DynamicMaterial = 
			UMaterialInstanceDynamic::Create(Material, nullptr);

		DynamicBrush.SetResourceObject(DynamicMaterial);
		DynamicBrush.ImageSize = FVector2D(64, 64);          // optional
		DynamicBrush.DrawAs    = ESlateBrushDrawType::Image; // must be set for material
		DynamicBrush.Tiling    = ESlateBrushTileType::NoTile;
		DynamicBrush.Mirroring = ESlateBrushMirrorType::NoMirror;
		DynamicBrush.TintColor = FSlateColor::UseForeground(); // optional
	}

	InitFromColor(InArgs._InitialColor.Get());

	TFunction<void(FVector2D)> SVChangedDelegate = [this](const FVector2D& SV)
	{
		Saturation = SV.X;
		Value      = SV.Y;
		UpdateColorFromHSV();
	};
	TFunction<void(float)> HueChangedDelegate = [this](const float NewHue)
	{
		Hue = NewHue;
		UpdateColorFromHSV();
	};

	
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(FMargin(10))
		[

			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.FillHeight(1.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.f)
				.Padding(0)
				[
					SNew(SColorPickerRectangle)
					.Style(Style)
					.HSV_Lambda([this]
					{
						return HSV;
					})
					.OnSVChanged(SVChangedDelegate)
				]
				+ SHorizontalBox::Slot()
				.Padding(FMargin(10, 0, 0, 0))
				.MaxWidth(35.f)
				.AutoWidth()
				.VAlign(VAlign_Fill)
				[
					SNew(SColorPickerSlider)
					.Value_Lambda([this]
					{
						return Hue;
					})
					.OnValueChanged(HueChangedDelegate)
					.Style(Style)
				]
			]
			+ SVerticalBox::Slot()
			.Padding(FMargin(0, 10, 0, 0))
			.MaxHeight(25.f)
			.HAlign(HAlign_Left)
			.AutoHeight()
			[

				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					MakeColorSpinBox(EColorChannel::R, FLinearColor::Red)
				]
				+ SHorizontalBox::Slot()
				[
					MakeColorSpinBox(EColorChannel::G, FLinearColor::Green)
				]
				+ SHorizontalBox::Slot()
				[
					MakeColorSpinBox(EColorChannel::B, FLinearColor::Blue)
				]
				+ SHorizontalBox::Slot()
				[
					MakeColorSpinBox(EColorChannel::A, FLinearColor::White)
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.MaxWidth(5.f)
					.AutoWidth()
					[
						SNew(SImage)
						.ColorAndOpacity(FLinearColor::Black)
					]
					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					.HAlign(HAlign_Fill)
					[
						SNew(SBox)
						.WidthOverride(42.f) // or whatever fixed width you want
						[
							SNew(SEditableTextBox)
							.MinDesiredWidth(42.0f)
							.Padding(FMargin(5, 0, 5, 0))
							.OnTextCommitted(this, &SGameColorPicker::OnHexStringCommitted)
							.Text(this, &SGameColorPicker::GetColorHexString)
							// .ToolTipText(LOCTEXT("MetadataKeyTooltipText", "The key value for the metadata."))
							// .HintText(LOCTEXT("MetadataKeyHint", "The key value for the metadata."))
						]
					]

				]
			]
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

SGameColorPicker::~SGameColorPicker()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->MarkAsGarbage();
	}
}

void SGameColorPicker::UpdateColorFromHSV()
{
	HSV = FLinearColor(Hue*360.0f, Saturation, Value, Alpha);

	RGB = HSV.HSVToLinearRGB();
	OnColorChanged.ExecuteIfBound(RGB);
	
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(FName("Hue"), Hue);
	}
}

void SGameColorPicker::OnHexStringCommitted(const FText& HexString, ETextCommit::Type Arg)
{
	if (Arg == ETextCommit::Type::OnEnter || Arg == ETextCommit::Type::OnUserMovedFocus)
	{
		FString CleanHex = HexString.ToString().Replace(TEXT("#"), TEXT("")).ToUpper();

		// Pad if needed
		while (CleanHex.Len() < 6)
		{
			CleanHex += TEXT("0");
		}

		uint32 HexValue = FParse::HexNumber(*CleanHex);

		uint8 R = (HexValue >> 16) & 0xFF;
		uint8 G = (HexValue >> 8) & 0xFF;
		uint8 B = HexValue & 0xFF;
		uint8 A = 255;

		// If alpha is included (8 digits)
		if (CleanHex.Len() == 8)
		{
			A = (HexValue >> 24) & 0xFF;
		}

		FColor SRGBColor(R, G, B, A);
		RGB = FLinearColor(SRGBColor); // Converts to linear color

		OnColorChanged.ExecuteIfBound(RGB);

		if (DynamicMaterial)
		{
			DynamicMaterial->SetScalarParameterValue(FName("Hue"), Hue);
		}
	}
}

TSharedRef<SWidget> SGameColorPicker::MakeColorSpinBox(EColorChannel Channel, FLinearColor DisplayColor)
{
	auto OnColorComponentChanged = [this,Channel](uint8 NewValue)
	{
		switch (Channel)
		{
		case EColorChannel::R:
			RGB.R = NewValue/255.f;
			break;
		case EColorChannel::G:
			RGB.G = NewValue/255.f;
			break;
		case EColorChannel::B:
			RGB.B = NewValue/255.f;
			break;
		case EColorChannel::A:
			RGB.A = NewValue/255.f;
			break;
		case EColorChannel::H:
			HSV.R = NewValue/360.f;
			break;
		case EColorChannel::S:
			HSV.G = NewValue;
			break;
		case EColorChannel::V:
			HSV.B = NewValue;
			break;
		}
	};

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.MaxWidth(5.f)
		.AutoWidth()
		[
			SNew(SImage)
			.ColorAndOpacity(DisplayColor)
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.f)
		.HAlign(HAlign_Fill)
		[
			SNew(SBox)
			.WidthOverride(50.f) // or whatever fixed width you want
			[
				SNew(SSpinBox<uint8>)
				.ContentPadding(FMargin(5, 0, 1, 0))
				.Clipping(EWidgetClipping::ClipToBounds)
				.Value_Lambda([this, Channel]
				{
					switch (Channel)
					{
					case EColorChannel::R:
						return RGB.R * 255;
					case EColorChannel::G:
						return RGB.G * 255;
					case EColorChannel::B:
						return RGB.B * 255;
					case EColorChannel::A:
						return RGB.A * 255;
					case EColorChannel::H:
						return HSV.R * 360;
					case EColorChannel::S:
						return HSV.G;
					case EColorChannel::V:
						return HSV.B;
					}
					return -1.f;
				})
				.OnValueChanged_Lambda(OnColorComponentChanged)
			]
		];
}
