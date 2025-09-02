#include "pch.h"
#ifndef SQL
#define SQL
#endif
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/exception.h>
#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include "type.h"
#include "database.h"
namespace MySQL
{
	// mysql server instances
	static DRIVER* driver;

	static CONN* conn;

	static PSTMT* pstmt;

	static STMT* stmt;

	static RSET* rset;

	Connector::Connector(const std::string& host, const std::string& user, const std::string& pass)
	{
		driver = sql::mysql::get_mysql_driver_instance();;
		stmt = nullptr; pstmt = nullptr; conn = nullptr; rset = nullptr;

		SetCredentials(host, user, pass);
	}
	Connector::~Connector()
	{
		try
		{
			if (conn)
			{
				conn->close(); // try to close connection with mysql server
				delete conn;
			}
			if (stmt)
			{
				stmt->close();
				delete stmt;
			}
			if (pstmt)
			{
				pstmt->close();
				delete pstmt;
			}
			if (rset)
			{
				rset->close();
				delete rset;;
			}
		}
		catch (...)
		{

		}
	}
	void Connector::SetCredentials(const std::string& host, const std::string& user, const std::string& pass)
	{
		credentials[0] = host;
		credentials[1] = user;
		credentials[2] = pass;
	}
	bool Connector::Connect() const
	{
		try
		{
			conn = driver->connect(credentials[0], credentials[1], credentials[2]);
		}
		catch (...)
		{
			if (conn)
				delete conn;
			DbgMsg("error Connect()");

			return false;
		}
		return true;
	}
	bool Connector::SetDB(const std::string& schema) const
	{
		if (!conn)
		{
			DbgMsg("error SetDB() not connected");
			return false;
		}
		try
		{
			conn->setSchema(schema);
		}
		catch (...)
		{
			DbgMsg("error SetDB()");
			return false;
		}
		return true;
	}
	bool Connector::Query(const std::string& query) const
	{
		if (!conn)
		{
			DbgMsg("error Query() not connected");
			return false;
		}
		try
		{
			stmt = conn->createStatement();
			stmt->execute(query);
		}
		catch (...)
		{
			DbgMsg("error Query()");
			return false;
		}
		return true;
	}
	bool Connector::Write(std::string fmt, const std::string& query, va_list va) const
	{
		if (fmt.empty() || query.empty()) return false;
		if (!conn)
		{
			DbgMsg("error Write() not connected");
			return false;
		}

		try
		{
			pstmt = conn->prepareStatement(query);
		}
		catch (...)
		{
			DbgMsg("error Write() query is not ok");
			return false; // if query is invalid and exception is thrown
		}
		/*va_list va;
		va_start(va, &query);*/

		int i = 1; //initial pos used in setType db functions

		int arg_d; // temp vars for row data
		double arg_f;
		std::string* arg_s;
		uint arg_u;
		bool arg_b;

		for (const auto& c : fmt)
		{
			switch (c)
			{
			case 'd':arg_d = va_arg(va, int); pstmt->setInt(i++, arg_d); break; // follow c-style format
			case 'f': arg_f = va_arg(va, double); pstmt->setDouble(i++, arg_f); break;
			case 'b': arg_b = va_arg(va, bool); pstmt->setBoolean(i++, arg_b); break;
			case 's': arg_s = va_arg(va, std::string*); pstmt->setString(i++, *arg_s); break;
			case 'u': arg_u = va_arg(va, uint); pstmt->setUInt(i++, arg_u); break;
			}
		}
		va_end(va);

		try
		{
			pstmt->executeUpdate();// try executing the query with set values
		}
		catch (...)
		{
			DbgMsg("error Write() args are not ok");
			return false;
		}
		return true;
	}

	bool Connector::Read(std::string fmt, const std::string& query, std::string& dst) const
	{
		if (!conn)
		{
			DbgMsg("error Read() not connected");
			return false;
		}
		Utils::Trim(fmt, "difubs"); // trim format only to passed charset
		if (fmt.empty()) return false;

		try
		{
			stmt = conn->createStatement();
			rset = stmt->executeQuery(query); //try executing query and if error, stop function
		}
		catch (...)
		{
			DbgMsg("error Read() query is not ok");
			return false;
		}

		size_t cols = fmt.size();

		while (rset->next()) // iterate through each row of res set
		{
			for (size_t i = 1; i <= cols; i++)
			{
				std::string data;
				switch (fmt[i - 1])
				{
				case 'd': case 'i': data = std::to_string(rset->getInt(i)); break;
				case 'f': data = std::to_string(rset->getDouble(i)); break;
				case 'u': data = std::to_string(rset->getUInt(i)); break;
				case 'b': data = std::to_string(rset->getBoolean(i)); break;
				case 's': data = rset->getString(i); break;
				}
				if (!data.empty() && i != cols)
				{
					data.push_back(','); // place after every column received in each row
				}

				dst.append(data);
			}
			if (!rset->isLast() || rset->isFirst() && rset->isLast())
				dst.push_back('|'); // insert after each row of data from the db
		}

		return !dst.empty();
	}
}