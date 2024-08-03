#include "session.h"
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/canvas.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <iostream>

int main()
{
	auto screen {ftxui::ScreenInteractive::Fullscreen()};
	
	static constexpr int s_canvasWidth {120};
	static constexpr int s_canvasHeight {100};

	// 'ftxui::Canvas' to render the main game screen
	ftxui::Canvas c {s_canvasWidth, s_canvasHeight};

	auto canvasElement {ftxui::canvas(c)};

	// 'ftxui::InputOption' creates settings to pass into 'ftxui::Component::Input()'
	ftxui::InputOption settings {ftxui::InputOption::Spacious()};

	std::string content {};
	std::string placeholder {"type here"};

	settings.content = &content;
	settings.placeholder = &placeholder;
	settings.multiline = false;

	// 'ftxui::Component::Input()' to allow keyboard input
	auto inputComponent {ftxui::Input(settings)};

	auto session {ftxui::Renderer(inputComponent, [&]
	{
		return ftxui::vbox({
			canvasElement | ftxui::border,

			// Sets the input component to the same width as the canvas
			inputComponent->Render() | ftxui::border | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, s_canvasWidth)
		});
	})};

	screen.Loop(session | ftxui::center);

	return 0;
}