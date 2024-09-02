#include "input_component.h"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <cctype>

namespace
{
	void deleteMostRecentWord(std::string& str)
	{
		if (!str.empty())
		{
			// Remove trailing spaces
			str = str.substr(0, str.find_last_not_of(' ') + 1);

			// A single word is remaining
			if (!str.empty() && str.find(' ') == std::string::npos)
			{
				str.clear();
				return;
			}
		
			auto pos {str.find_last_of(' ') + 1};

			// Erase backwards up until a space character
			if (pos != std::string::npos)
				str.erase(pos, str.length() - pos);
		}
	}
}

namespace Keywords
{
	InputComponent::InputComponent(std::string_view placeholder)
		: placeholder {placeholder}
	{
		ftxui::InputOption settings {};

		// Set the default settings 
		settings.content = &content;
		settings.placeholder = this->placeholder;
		settings.multiline = false;

		// Format the underlying element 
		settings.transform = [&] (ftxui::InputState state)
		{
			// Change the attributes of the placeholder text
			if (state.is_placeholder)
				(state.element |= ftxui::color(ftxui::Color::Grey30)) |= ftxui::dim;
			else
				state.element |= ftxui::bold;

			// Offset and center the element
			state.element |= ftxui::borderEmpty;
			state.element |= ftxui::vcenter;

			return state.element;
		};

		// Apply the above settings
		component = ftxui::Input(settings);

		// Filter out digit, uppercase, and non-letter characters
		component |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			return event.is_character() && std::isupper(event.character()[0])
				|| event.is_character() && !std::isalpha(event.character()[0]) 
				&& event.character()[0] != ' ';
		});

		// Register when the 'ENTER' key is pressed
		component |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			hasPressedEnter = (event == ftxui::Event::Return ? true : false);

			return hasPressedEnter;
		});

		// Bind 'CTRL + W' to the deletion of the last typed word
		component |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			const auto ctrlW {ftxui::Event::Special("\x17")};
			
			if (event == ctrlW)
			{
				deleteMostRecentWord(content);
				return true;
			}

			return false;
		});
	}
}