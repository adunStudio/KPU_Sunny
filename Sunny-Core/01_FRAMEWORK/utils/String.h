#pragma once

#include "../include.h"

namespace sunny
{
	namespace utils
	{
		int FindStringPosition(const std::string& string, const std::string search, unsigned int offset = 0);
		
		std::string StringRange(const std::string& string, unsigned int start, unsigned int length);
		std::string RemoveStringRange(const std::string& string, unsigned int start, unsigned int length);
		
		const char* FindToken(const char* str, const std::string& token);
		const char* FindToken(const std::string& string, const std::string token);

		std::string GetBlock(const char* str, const char** outPosition = nullptr);
		std::string GetBlock(const std::string& string, unsigned int offset = 0);

		std::string GetStatement(const char* str, const char** outPosition = nullptr);


		std::vector<std::string> SplitString(const std::string& string, const std::string& delimiters);
		std::vector<std::string> SplitString(const std::string& string, const char delimiter);
		std::vector<std::string> Tokenize(const std::string& string);
		std::vector<std::string> GetLines(const std::string& string);


		bool StringContains(const std::string& string, const std::string& chars);
		
		bool StartsWith(const std::string& string, const std::string& start);
		
		int NextInt(const std::string& string);
	}
}