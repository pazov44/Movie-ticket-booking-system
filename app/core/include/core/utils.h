#pragma once

namespace Utils
{
	/// @brief Exit the program with an error message.
	void Exit();

	/// @brief Display an error message.
	/// @param msg The error message to show.
	/// @param sec Duration in seconds to display the message (default is 0).
	void ErrMsg(const std::string& msg, int sec = 0);

	/// @brief Clears the command prompt screen.
	void Clear();

	/// @brief Trims the source string based on specified characters.
	/// @param src The string to trim.
	/// @param chars Characters to use for trimming.
	/// @param possible If true, only characters matching 'chars' are left.
	///                 If false, characters in 'chars' are removed.
	void Trim(std::string& src, const std::string& chars, bool possible = true);
}
