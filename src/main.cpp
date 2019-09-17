#include <iostream>
#include <cstdint>
#include "cairo.h"


const int PixelsPerWord = 32;


int DivRoundUp(int Numerator, int Denominator)
{
	return ((Numerator - 1) / Denominator) + 1;
}


cairo_surface_t* CreateImageSurface(const int IdealWidth, const int Height)
{
	const int WordsPerRow = DivRoundUp(IdealWidth, PixelsPerWord);
	const int Width = WordsPerRow * PixelsPerWord;
	std::cout << "Image Size: " << Width << " by " << Height << "\n";
	return cairo_image_surface_create(CAIRO_FORMAT_A1, Width, Height);
}


void ClearImage(cairo_surface_t* Surface)
{
	const int Width = cairo_image_surface_get_width(Surface);
	const int Height = cairo_image_surface_get_height(Surface);
	const int WordsPerRow = Width / PixelsPerWord;
	const int TotalWords = Height * WordsPerRow;
	uint32_t* ImageData = (uint32_t*)cairo_image_surface_get_data(Surface);

	cairo_surface_flush(Surface);
	for (int w = 0; w < TotalWords; ++w)
	{
		ImageData[w] = 0;
	}
	cairo_surface_mark_dirty(Surface);
}


void PrintImage(cairo_surface_t* Surface)
{
	cairo_surface_flush(Surface);
	const int Width = cairo_image_surface_get_width(Surface);
	const int Height = cairo_image_surface_get_height(Surface);
	const int WordsPerRow = Width / PixelsPerWord;
	uint32_t* ImageData = (uint32_t*)cairo_image_surface_get_data(Surface);
	for (int y = 0; y < Height; ++y)
	{
		const int WordsSoFar = y * WordsPerRow;
		for (int x = 0; x < Width; ++x)
		{
			const int WordInRow = x / PixelsPerWord;
			const int BitOffset = x % PixelsPerWord;
			const int Word = WordsSoFar + WordInRow;
			const bool FG = (ImageData[Word] & (1 << BitOffset)) > 0;
			std::cout << (FG ? "#" : "-");
		}
		std::cout << "\n";
	}
}


void HailEris(cairo_surface_t* Surface)
{
	const int Width = cairo_image_surface_get_width(Surface);
	const int Height = cairo_image_surface_get_height(Surface);
	{
		cairo_t* Ctx = cairo_create(Surface);
		cairo_set_source_rgba(Ctx, 1.0, 1.0, 1.0, 1.0);
		cairo_select_font_face(Ctx, "monospace", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(Ctx, 20.0);
		cairo_move_to(Ctx, 4.0, Height - 2.0);
		cairo_show_text(Ctx, "Hail Eris!");
	}
}


int main()
{
	cairo_surface_t* Surface = CreateImageSurface(80, 20);
	ClearImage(Surface);
	HailEris(Surface);
	PrintImage(Surface);
	return 0;
}