#include "session.h"

namespace Keywords
{
	ftxui::Element Session::draw()
	{
		// Create a temporary 'ftxui::Canvas' 
		// this removes the need to clear it since each 
		// time this is called a blank canvas is created

		// Call 'Session::update()' post draw phase

	}

	void Session::update()
	{
		// Loops through 'm_words', and increments their positions
		// Calls 'addWords()' and 'eraseWords()' (the latter only on 'ENTER')


	}

	void Session::addWords()
	{
		// Reads in from 'WordBank', then creates new 'Word' object
		// Adds a given amount of 'Word' objects to 'm_words'


	}

	void Session::eraseWords()
	{
		// Loops through 'm_words' and checks against 'Word::m_content'
		// if they match 'InputComponent::getContent()', they are erased


	}
}