#ifndef KEYWORDS_INPUT_COMPONENT_H
#define KEYWORDS_INPUT_COMPONENT_H

#include <ftxui/dom/node.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <string_view>

namespace Keywords
{
	struct InputComponent
	{
		InputComponent(std::string_view placeholder = "type here");

		ftxui::Element draw() const { return component->Render(); }
		void reset() { content.clear(); }
		
		std::string content {};
		const std::string placeholder {};
		ftxui::Component component {};
		bool hasPressedEnter {};
		bool hasPressedEscape {};
	};
}

#endif