#ifndef SHADOW_TEXT_INCLUDE
#define SHADOW_TEXT_INCLUDE

#include "GuiElement.h"

namespace sh
{

namespace gui
{

	class Text : public GuiElement
	{
	public:
		Text(); 
		Text(const math::Rectu rect);
		Text(const String& text);
		Text(const math::Rectu rect, const String& text);

		virtual ~Text();

		void SetText(const String& text);
		const String& GetText() const { return m_text; }

		virtual void GetTextGeometry(GuiBatchData& data) override;

	protected:
		void UpdateTextGeometry();

	protected:
		String m_text;
		std::vector<float> m_textBatchData;
	};

} // gui

} // sh

#endif