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
		
			auto pos {str.find_last_of(' ')};

			// Erase backwards up until a space character
			if (pos != std::string::npos)
				str.erase(pos, str.length() - pos);
		}
	}
}

namespace Keywords
{
	InputComponent::InputComponent()
		: m_placeholder {"type here"}
	{
		auto settings {ftxui::InputOption::Spacious()};

		// Create the default settings for 'm_input'
		settings.content = &m_content;
		settings.placeholder = &m_placeholder;
		settings.multiline = false;

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
			static auto ctrlW {ftxui::Event::Special("\x17")};
			
			if (event == ctrlW)
			{
				deleteMostRecentWord(m_content);
				return true;
			}

			return false;
		});
	}
}