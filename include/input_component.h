#ifndef KEYWORDS_INPUT_COMPONENT_H
#define KEYWORDS_INPUT_COMPONENT_H

#include <ftxui/dom/node.hpp>
#include <ftxui/component/component.hpp>
#include <string>

namespace Keywords
{
	class InputComponent
	{
	public:
		// Sets up 'ftxui::Component::Input()' via 'ftxui::InputOptions'
		InputComponent();

		ftxui::Element draw() { return m_input->Render(); }
		void reset() { m_content = ""; }
		bool hasPressedEnter() { return m_hasPressedEnter; };
		
		const std::string& getContent() { return m_content; }
		const ftxui::Component& getComponent() { return m_input; }

	private:
		std::string m_content {};
		std::string m_placeholder {};
		ftxui::Component m_input {};
		bool m_hasPressedEnter {};

	};
}

#endif