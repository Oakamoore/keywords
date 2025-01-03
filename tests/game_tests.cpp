#include "game_config.h"
#include "util.h"
#include "constants.h"
#include <catch2/catch_test_macros.hpp>
#include <iterator>

TEST_CASE("Determine whether game audio is disabled")
{
	SECTION("No arguments are passed in")
	{
		const char* arguments[] {nullptr};

		Keywords::GameConfig config {Keywords::Util::isArgumentPresent(std::size(arguments), const_cast<char**>(arguments), Keywords::Constants::disableAudio)};

		REQUIRE(config.isAudioEnabled == false);
	}

	SECTION("Expected argument is passed in")
	{
		// The first element represents the program's name, and is ignored
		const char* arguments[] {"", Keywords::Constants::disableAudio.data()};

		Keywords::GameConfig config {Keywords::Util::isArgumentPresent(std::size(arguments), const_cast<char**>(arguments), Keywords::Constants::disableAudio)};

		REQUIRE(config.isAudioEnabled == true);
	}

	SECTION("Expected argument isn't passed in")
	{
		const char* arguments[] {"", "no", "audio"};

		Keywords::GameConfig config {Keywords::Util::isArgumentPresent(std::size(arguments), const_cast<char**>(arguments), Keywords::Constants::disableAudio)};

		REQUIRE(config.isAudioEnabled == false);
	}
}
