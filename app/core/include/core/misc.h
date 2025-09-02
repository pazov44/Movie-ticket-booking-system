#pragma once
#include <unordered_map>

enum Error
{
	ERROR_INPUT,
	ERROR_DATABASE,
	ERROR_EXISTS,
	ERROR_NOT_EXISTS,
	ERROR_FAILED,
	SUCCESSFUL
};

enum Seat
{
	SILVER =1,
	GOLD,
	PLATINUM
};

extern std::unordered_map<Seat, int> seatPrice;
extern std::unordered_map<Seat, std::string> seatType;

namespace Misc
{
	/// @brief Inserts data into the database using a formatted query.
/// @param fmt Format string describing the data format.
/// @param query SQL insert query with placeholders.
/// @param ... Variable arguments corresponding to the placeholders in the query.
/// @return Status code (e.g., 0 for success, non-zero for failure).
	int Insert(const std::string& fmt, const std::string& query, ...);

	/// @brief Deletes data from the database conditionally.
	/// @param fmt Format string describing the data format.
	/// @param querySelect SQL select query to validate the condition.
	/// @param queryDelete SQL delete query to perform the deletion.
	/// @param checkDstEmpty If true, deletion proceeds only if destination is empty.
	/// @return Status code (e.g., 0 for success, non-zero for failure).
	int Delete(const std::string& fmt, const std::string& querySelect, const std::string& queryDelete, bool checkDstEmpty = false);

	/// @brief Selects data from the database and stores it in a destination string.
	/// @param fmt Format string describing the expected data format.
	/// @param query SQL select query to execute.
	/// @param dst String to store the query result.
	/// @return Status code (e.g., 0 for success, non-zero for failure).
	int Select(const std::string& fmt, const std::string& query, std::string& dst);

	/// @brief Executes an update operation in the database.
	/// @param query SQL update query to execute.
	/// @return True if the update was successful, false otherwise.
	bool Update(const std::string& query);

	/// @brief Validates a credit card number using the Luhn algorithm.
	/// @param number Credit card number as a string.
	/// @return True if the number is valid, false otherwise.
	bool LuhnCheck(const std::string& number);
}



