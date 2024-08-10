#ifndef KEYWORDS_INPUT_COMPONENT_H
#define KEYWORDS_INPUT_COMPONENT_H

#include <ftxui/dom/node.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <string_view>

namespace Keywords
{
	class InputComponent
	{
	public:
		InputComponent(std::string_view placeholder = "type here");

		ftxui::Element draw() { return m_input->Render(); }
		
		void reset() { m_content.clear(); }
		bool hasPressedEnter() const { return m_hasPressedEnter; };
		
		void setContent(std::string_view content) { m_content = content; }
		const std::string& getContent() const { return m_content; }
		
		const ftxui::Component& getComponent() const { return m_input; }

	private:
		std::string m_content {};
		std::string m_placeholder {"type here"};
		ftxui::Component m_input {};
		bool m_hasPressedEnter {};

	};
}

#endif