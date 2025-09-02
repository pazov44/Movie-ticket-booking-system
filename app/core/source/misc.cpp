#include "pch.h"
std::unordered_map<Seat, int> seatPrice
{
	{SILVER,5},
	{GOLD,10},
	{PLATINUM,15}
};
std::unordered_map<Seat, std::string> seatType
{
	{SILVER,"Silver"},
	{GOLD,"Gold"},
	{PLATINUM,"Platinum"}
};

namespace Misc
{
	int Insert(const std::string& fmt, const std::string& query, ...)
	{
		auto shsqlInst = Manager::GetSQL()->GetInstance();
		va_list va;
		va_start(va, &query);

		return shsqlInst->Write(fmt, query, va) ? Error::SUCCESSFUL : Error::ERROR_EXISTS;
	}
	int Delete(const std::string& fmt, const std::string& querySelect, const std::string& queryDelete, bool checkDstEmpty)
	{
		auto shsqlInst = Manager::GetSQL()->GetInstance();
		std::string dstRead;

		if (shsqlInst->Read(fmt, querySelect, dstRead)) // select query to check if data exists because delete from always executes
		{
			if (checkDstEmpty && dstRead.empty()) return Error::ERROR_FAILED;

			if (shsqlInst->Query(queryDelete))
			{
				return Error::SUCCESSFUL;
			}
		}
		return Error::ERROR_FAILED;
	}
	int Select(const std::string& fmt, const std::string& query, std::string& dst)
	{
		auto shsqlInst = Manager::GetSQL()->GetInstance();
		return shsqlInst->Read(fmt, query, dst) ? Error::SUCCESSFUL : Error::ERROR_NOT_EXISTS;
	}
	bool Update(const std::string& query)
	{
		auto shsqlInst = Manager::GetSQL()->GetInstance();
		return shsqlInst->Query(query);
	}
	bool LuhnCheck(const std::string& number)
	{
		int sum = 0;
		bool alternate = false;
		for (int i = number.length() - 1; i >= 0; i--)
		{
			int n = number[i] - '0';
			if (alternate) {
				n *= 2;
				if (n > 9) n -= 9;
			}
			sum += n;
			alternate = !alternate;
		}
		return (sum % 10 == 0);
	}
}