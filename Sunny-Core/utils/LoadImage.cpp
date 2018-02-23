#include "LoadImage.h"
#include "../system/VFS.h"

// freeimage 로드
#include "../dependency/freeimage/x64/FreeImage.h"


#if _WIN32 || _WIN64
#if _WIN64
#pragma comment(lib, "dependency/freeimage/x64/FreeImage.lib")
#else
#pragma comment(lib, "dependency/freeimage/x32/FreeImage.lib")
#endif
#endif

template <class T> void INPLACESWAP(T& a, T& b) { a ^= b; b ^= a; a ^= b; };
BOOL SwapRedBlue32(FIBITMAP* dib);

namespace sunny
{
	namespace utils
	{
		unsigned char* LoadSunnyImage(const char*  filename, unsigned int* width, unsigned int* height, unsigned int* bits, bool flipY)
		{

			std::string physicalPath;

			// 파일 존재유무 확인
			if (!VFS::Get()->ResolvePhysicalPath(filename, physicalPath))
			{
				std::cout << "Could not load image '" << filename << "'!" << std::endl;
				return nullptr;
			}


			filename = physicalPath.c_str();

			FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
			FIBITMAP* dib = nullptr;
			
			fif = FreeImage_GetFileType(filename, 0);


			if (fif == FIF_UNKNOWN)
				fif = FreeImage_GetFIFFromFilename(filename);
			if (fif == FIF_UNKNOWN)
				return nullptr;

			if (FreeImage_FIFSupportsReading(fif))
				dib = FreeImage_Load(fif, filename);

			if (!dib)
			{
				// Debug System
				std::cout << "Could not load image '" << filename << "'!" << std::endl;
				return nullptr;
			}

			FIBITMAP* bitmap = FreeImage_ConvertTo32Bits(dib);
			FreeImage_Unload(dib);

			unsigned char* pixels = FreeImage_GetBits(bitmap);
			unsigned int w = FreeImage_GetWidth(bitmap);
			unsigned int h = FreeImage_GetHeight(bitmap);
			unsigned int b = FreeImage_GetBPP(bitmap);
		
			// 뒤집기
			if (flipY)
				FreeImage_FlipVertical(bitmap);

			if (FreeImage_GetRedMask(bitmap) == 0xff0000)
				SwapRedBlue32(bitmap);

			if (width)
				*width = w;
			if (height)
				*height = h;
			if (bits)
				*bits = b;

			unsigned int size = w * h * (b / 8);
			
			unsigned char* result = new unsigned char[size];
			
			memcpy(result, pixels, size);
			
			FreeImage_Unload(bitmap);
			
			return result;
		}

		unsigned char* LoadSunnyImage(const std::string&  filename, unsigned int* width, unsigned int* height, unsigned int* bits, bool flipY)
		{
			return LoadSunnyImage(filename.c_str(), width, height, bits, flipY);
		}
	}
}

BOOL SwapRedBlue32(FIBITMAP* dib) {
	if (FreeImage_GetImageType(dib) != FIT_BITMAP) {
		return FALSE;
	}

	const unsigned bytesperpixel = FreeImage_GetBPP(dib) / 8;
	if (bytesperpixel > 4 || bytesperpixel < 3) {
		return FALSE;
	}

	const unsigned height = FreeImage_GetHeight(dib);
	const unsigned pitch = FreeImage_GetPitch(dib);
	const unsigned lineSize = FreeImage_GetLine(dib);

	BYTE* line = FreeImage_GetBits(dib);
	for (unsigned y = 0; y < height; ++y, line += pitch) {
		for (BYTE* pixel = line; pixel < line + lineSize; pixel += bytesperpixel) {
			INPLACESWAP(pixel[0], pixel[2]);
		}
	}

	return TRUE;
}