#pragma once
#ifdef SQL
typedef sql::PreparedStatement PSTMT;
typedef sql::ResultSet RSET;
typedef sql::Statement STMT;
typedef sql::Connection CONN;
typedef sql::mysql::MySQL_Driver DRIVER;
#endif
typedef unsigned char byte;
typedef unsigned int uint;

