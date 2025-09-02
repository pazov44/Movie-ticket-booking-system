#pragma once
#include <string>

namespace MySQL
{
	/**
	 * @class Connector
	 * @brief Handles MySQL database connections and operations.
	 *
	 * This class manages connection credentials, schema selection, and
	 * query execution for a MySQL database.
	 */
	class Connector
	{
		/// @brief Array holding host, username, and password credentials.
		std::string credentials[3];
	public:
		/**
		 * @brief Constructs a new Connector instance with connection credentials.
		 *
		 * @param host MySQL server address.
		 * @param user Username for authentication.
		 * @param pass Password for authentication.
		 */
		Connector(const std::string& host, const std::string& user, const std::string& pass);

		/**
		 * @brief Destructor for Connector.
		 */
		~Connector();

		/**
		 * @brief Sets the active database schema to use.
		 *
		 * @param schema The name of the schema/database.
		 * @return true If the schema was successfully set.
		 * @return false If setting the schema failed.
		 */
		bool SetDB(const std::string& schema) const;

		/**
		 * @brief Establishes a connection to the MySQL server.
		 *
		 * @return true If connection was successful.
		 * @return false If connection failed.
		 */
		bool Connect() const;
		/**
		 * @brief Executes a SELECT query and reads data using a C-style format string.
		 *
		 * @param fmt Format string specifying expected column types.
		 * @param query The SQL SELECT query.
		 * @param dst Output string to store the result.
		 * @return true If data was successfully read.
		 * @return false If the query or read failed.
		 */
		bool Read(std::string fmt, const std::string& query, std::string& dst) const;

		/**
		 * @brief Executes an INSERT query using a C-style format string and variadic arguments.
		 *
		 * @param fmt Format string indicating parameter types.
		 * @param query The SQL INSERT query.
		 * @param va List of variadic arguments matching the format.
		 * @return true If the write was successful.
		 * @return false If the insert failed.
		 */
		bool Write(std::string fmt, const std::string& query, va_list va) const;

		/**
		 * @brief Executes a generic SQL query.
		 *
		 * @param query The SQL statement (e.g., UPDATE, DELETE, etc.).
		 * @return true If the query was executed successfully.
		 * @return false If the query execution failed.
		 */
		bool Query(const std::string& query) const;

		/**
		 * @brief Sets or updates the database connection credentials.
		 *
		 * @param host MySQL server address.
		 * @param user Username for authentication.
		 * @param pass Password for authentication.
		 */
		void SetCredentials(const std::string& host, const std::string& user, const std::string& pass);
	};
}