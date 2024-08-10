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
		: m_placeholder {placeholder}
	{
		ftxui::InputOption settings {};

		// Set the default settings for 'm_input'
		settings.content = &m_content;
		settings.placeholder = &m_placeholder;
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

		// Apply the above settings to 'm_input'
		m_input = ftxui::Input(settings);

		// Filter out digit, uppercase, and non-letter characters
		m_input |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			return event.is_character() && std::isupper(event.character()[0])
				|| event.is_character() && !std::isalpha(event.character()[0]) 
				&& event.character()[0] != ' ';
		});

		// Register when the 'ENTER' key is pressed
		m_input |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			m_hasPressedEnter = (event == ftxui::Event::Return ? true : false);

			return m_hasPressedEnter;
		});

		// Bind 'CTRL + W' to the deletion of the last typed word in 'm_content'
		m_input |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			const static auto ctrlW {ftxui::Event::Special("\x17")};
			
			if (event == ctrlW)
			{
				deleteMostRecentWord(m_content);
				return true;
			}

			return false;
		});
	}
}