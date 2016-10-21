#ifndef SHADOW_COLOR_INCLUDE
#define SHADOW_COLOR_INCLUDE

#include "../math/shMath.h"

namespace sh
{
	namespace video
	{
		
		class Color
		{
		public:
			enum class ColorFormat
			{
				//! 16 bit color format used by the software driver.
				/** It is thus preferred by all other irrlicht engine video drivers.
				There are 5 bits for every color component, and a single bit is left
				for alpha information. */
				A1R5G5B5 = 0,

				//! Standard 16 bit color format.
				R5G6B5,

				//! 24 bit color, no alpha channel, but 8 bit for red, green and blue.
				R8G8B8,

				//! Default 32 bit color format. 8 bits are used for every component: red, green, blue and alpha.
				A8R8G8B8,

				/** Compressed image formats. **/

				//! DXT1 color format.
				DXT1,

				//! DXT2 color format.
				DXT2,

				//! DXT3 color format.
				DXT3,

				//! DXT4 color format.
				DXT4,

				//! DXT5 color format.
				DXT5,

				//! PVRTC RGB 2bpp.
				PVRTC_RGB2,

				//! PVRTC ARGB 2bpp.
				PVRTC_ARGB2,

				//! PVRTC RGB 4bpp.
				PVRTC_RGB4,

				//! PVRTC ARGB 4bpp.
				PVRTC_ARGB4,

				//! PVRTC2 ARGB 2bpp.
				PVRTC2_ARGB2,

				//! PVRTC2 ARGB 4bpp.
				PVRTC2_ARGB4,

				//! ETC1 RGB.
				ETC1,

				//! ETC2 RGB.
				ETC2_RGB,

				//! ETC2 ARGB.
				ETC2_ARGB,

				/** The following formats may only be used for render target textures. */

				/** Floating point formats. */

				//! 16 bit format using 16 bits for the red channel.
				R16F,

				//! 32 bit format using 16 bits for the red and green channels.
				G16R16F,

				//! 64 bit format using 16 bits for the red, green, blue and alpha channels.
				A16B16G16R16F,

				//! 32 bit format using 32 bits for the red channel.
				R32F,

				//! 64 bit format using 32 bits for the red and green channels.
				G32R32F,

				//! 128 bit format using 32 bits for the red, green, blue and alpha channels.
				A32B32G32R32F,

				/** Unsigned normalized integer formats. */

				//! 8 bit format using 8 bits for the red channel.
				R8,

				//! 16 bit format using 8 bits for the red and green channels.
				R8G8,

				//! 16 bit format using 16 bits for the red channel.
				R16,

				//! 32 bit format using 16 bits for the red and green channels.
				R16G16,

				/** Depth and stencil formats. */

				//! 16 bit format using 16 bits for depth.
				D16,

				//! 32 bit format using 32 bits for depth.
				D32,

				//! 32 bit format using 24 bits for depth and 8 bits for stencil.
				D24S8,

				//! Unknown color format:
				UNKNOWN
			};

			Color() : color(0U) {}

			//! Constructs the color from 4 values representing the alpha, red, green and blue component.
			/** Must be values between 0 and 255. */
			Color(u32 a, u32 r, u32 g, u32 b)
				: color(((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF)) {}

			Color(u32 other)
				: color(other) {}

			//! Returns the alpha component of the color.
			/** The alpha component defines how opaque a color is.
			\return The alpha value of the color. 0 is fully transparent, 255 is fully opaque. */
			u32 GetAlpha() const { return color >> 24; }

			//! Returns the red component of the color.
			/** \return Value between 0 and 255, specifying how red the color is.
			0 means no red, 255 means full red. */
			u32 GetRed() const { return (color >> 16) & 0xFF; }

			//! Returns the green component of the color.
			/** \return Value between 0 and 255, specifying how green the color is.
			0 means no green, 255 means full green. */
			u32 GetGreen() const { return (color >> 8) & 0xFF; }

			//! Returns the blue component of the color.
			/** \return Value between 0 and 255, specifying how blue the color is.
			0 means no blue, 255 means full blue. */
			u32 GetBlue() const { return color & 0xFF; }

			

			//! Sets the alpha component of the Color.
			/** The alpha component defines how transparent a color should be.
			\param a The alpha value of the color. 0 is fully transparent, 255 is fully opaque. */
			void SetAlpha(u32 a) { color = ((a & 0xff) << 24) | (color & 0x00FFFFFF); }

			//! Sets the red component of the Color.
			/** \param r: Has to be a value between 0 and 255.
			0 means no red, 255 means full red. */
			void SetRed(u32 r) { color = ((r & 0xff) << 16) | (color & 0xFF00FFFF); }

			//! Sets the green component of the Color.
			/** \param g: Has to be a value between 0 and 255.
			0 means no green, 255 means full green. */
			void SetGreen(u32 g) { color = ((g & 0xff) << 8) | (color & 0xFFFF00FF); }

			//! Sets the blue component of the Color.
			/** \param b: Has to be a value between 0 and 255.
			0 means no blue, 255 means full blue. */
			void SetBlue(u32 b) { color = (b & 0xff) | (color & 0xFFFFFF00); }

			//! Converts color to OpenGL color format
			/** From ARGB to RGBA in 4 byte components for endian aware
			passing to OpenGL
			\param dest: address where the 4x8 bit OpenGL color is stored. */
			void ToOpenGLColor(u8* dest) const
			{
				*dest = (u8)GetRed();
				*++dest = (u8)GetGreen();
				*++dest = (u8)GetBlue();
				*++dest = (u8)GetAlpha();
			}

			void Set(u32 a, u32 r, u32 g, u32 b)
			{
				color = (((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF));
			}
			void Set(u32 col) { color = col; }

			//! Compares the color to another color.
			/** \return True if the colors are the same, and false if not. */
			bool operator==(const Color& other) const { return other.color == color; }

			//! Compares the color to another color.
			/** \return True if the colors are different, and false if they are the same. */
			bool operator!=(const Color& other) const { return other.color != color; }

			//! comparison operator
			/** \return True if this color is smaller than the other one */
			bool operator<(const Color& other) const { return (color < other.color); }

			//! Adds two colors, result is clamped to 0..255 values
			/** \param other Color to add to this color
			\return Addition of the two colors, clamped to 0..255 values */
			Color operator+(const Color& other) const
			{
				return Color(math::min_(GetAlpha() + other.GetAlpha(), 255U),
					math::min_(GetRed() + other.GetRed(), 255U),
					math::min_(GetGreen() + other.GetGreen(), 255U),
					math::min_(GetBlue() + other.GetBlue(), 255U));
			}

			u32 color;
		};






		class Colorf
		{
		public:
			Colorf() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
			Colorf(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}
			void Set(f32 rr, f32 gg, f32 bb) { r = rr; g = gg; b = bb; }
			void Set(f32 aa, f32 rr, f32 gg, f32 bb) { a = aa; r = rr; g = gg; b = bb; }		

			//! Returns the red component of the color in the range 0.0 to 1.0
			f32 GetRed() const { return r; }

			//! Returns the green component of the color in the range 0.0 to 1.0
			f32 GetGreen() const { return g; }

			//! Returns the blue component of the color in the range 0.0 to 1.0
			f32 GetBlue() const { return b; }

			//! Returns the alpha component of the color in the range 0.0 (transparent) to 1.0 (opaque)
			f32 GetAlpha() const { return a; }

			f32 r;
			f32 g;
			f32 b;
			f32 a;
		};
	}
}
#endif