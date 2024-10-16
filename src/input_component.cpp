#include "input_component.h"
#include "constants.h"
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <cctype>

namespace Keywords
{
	InputComponent::InputComponent(std::string_view placeholder)
		: placeholder {placeholder}
	{
		ftxui::InputOption settings {};

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

			// Widen and center the element
			state.element |= ftxui::borderEmpty;
			state.element |= ftxui::center;

			return state.element;
		};

		// Apply the above settings
		component = ftxui::Input(settings);

		// Filter out digit, uppercase, and non-letter characters
		component |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			return event.is_character() && std::isupper(event.character()[0]) ||
				event.is_character() && !std::isalpha(event.character()[0]);
		});

		// Register when the 'ENTER' key is pressed
		component |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			hasPressedEnter = (event == ftxui::Event::Return ? true : false);

			return hasPressedEnter;
		});

		// Register when the 'ESCAPE' key is pressed
		component |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			hasPressedEscape = (event == ftxui::Event::Escape ? true : false);

			return hasPressedEscape;
		});

		// Bind a given key combination to word deletion
		component |= ftxui::CatchEvent([&] (ftxui::Event event)
		{
			if (event == Constants::wordDeletionEvent)
			{
				reset();
				return true;
			}

			return false;
		});
	}
}