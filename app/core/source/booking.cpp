#include "pch.h"
#include "models/booking.h"
#include "misc.h"

namespace Entity
{
	int Booking::Delete()
	{
		return Misc::Delete("%s", "SELECT USERID FROM BOOKINGS WHERE SEATX = " + std::to_string(seatX) + " AND SEATY = " + std::to_string(seatY),
			"DELETE FROM BOOKINGS WHERE SEATX = " + std::to_string(seatX) + " AND SEATY = " + std::to_string(seatY));
	}
	int Booking::Insert()
	{
		return Misc::Insert("%d %f %d %d %d %s %d", "INSERT INTO BOOKINGS(SHOWID,FINALPRICE,USERID,SEATX,SEATY,SEATTYPE,HALLNUMBER) VALUES(?,?,?,?,?,?,?)", showId, finalPrice, userId, seatX, seatY, &seatType, hallNumber);
	}
	int Booking::Select(std::vector<Entity::Booking>& bookings)
	{
		int ret = Error::ERROR_FAILED;

		std::string query = "SELECT SHOWID,FINALPRICE,USERID,SEATX,SEATY,SEATTYPE,HALLNUMBER FROM BOOKINGS";

		if (showId > 0)
		{
			query.append(" WHERE SHOWID = " + std::to_string(showId));
			if (hallNumber > 0)
			{
				query += " AND HALLNUMBER = " + std::to_string(hallNumber);
			}
		}
		else
		{
			return ret;
		}

		std::string dst;
		ret = Error::ERROR_NOT_EXISTS;

		if (Misc::Select("%d %f %d %d %d %s %d", query, dst) == Error::SUCCESSFUL && !dst.empty())
		{
			size_t start = 0, end;
			while ((end = dst.find('|', start)) != std::string::npos)
			{
				std::string record = dst.substr(start, end - start);
				start = end + 1;

				std::stringstream ss(record);
				std::string token;
				Entity::Booking book;

				if (std::getline(ss, token, ',')) book.SetShowId(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetFinalPrice(std::stof(token));
				if (std::getline(ss, token, ',')) book.SetUserId(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetSeatX(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetSeatY(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetSeatType(token);
				if (std::getline(ss, token, ',')) book.SetHallNumber(std::stoi(token));

				bookings.push_back(book);
				DbgMsg("SelectBookings() : %d %f %d %d %d %s %d", book.GetShowId(), book.GetFinalPrice(), book.GetUserId(), book.GetSeatX(), book.GetSeatY(), book.GetSeatType(), book.GetHallNumber());
			}
			if (start < dst.length())
			{
				std::stringstream ss(dst.substr(start));
				std::string token;
				Entity::Booking book;

				if (std::getline(ss, token, ',')) book.SetShowId(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetFinalPrice(std::stof(token));
				if (std::getline(ss, token, ',')) book.SetUserId(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetSeatX(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetSeatY(std::stoi(token));
				if (std::getline(ss, token, ',')) book.SetSeatType(token);
				if (std::getline(ss, token, ',')) book.SetHallNumber(std::stoi(token));


				bookings.push_back(book);
				DbgMsg("SelectBookings() : %d %f %d %d %d %s %d", book.GetShowId(), book.GetFinalPrice(), book.GetUserId(), book.GetSeatX(), book.GetSeatY(), book.GetSeatType(), book.GetHallNumber());
			}

			ret = bookings.empty() ? Error::ERROR_NOT_EXISTS : Error::SUCCESSFUL;
		}

		return ret;
	}
	int Booking::Select(std::string& dst)
	{
		int ret = Error::ERROR_NOT_EXISTS;

		std::string raw;
		std::string query = "SELECT SHOWID,FINALPRICE,SEATX,SEATY,SEATTYPE,HALLNUMBER FROM BOOKINGS WHERE USERID = '" + std::to_string(userId) + "'";

		if (Misc::Select("%d %f %d %d %s %d", query, raw) == Error::SUCCESSFUL && !raw.empty())
		{
			std::stringstream input(raw);
			std::stringstream output;
			std::string record;

			while (std::getline(input, record, '|'))
			{
				std::stringstream row(record);
				std::string token;
				int fieldIndex = 0;

				while (std::getline(row, token, ','))
				{
					if (fieldIndex == 1)
					{
						float price = std::stof(token);
						output << std::fixed << std::setprecision(2) << price;
					}
					else
					{
						output << token;
					}

					if (++fieldIndex < 6) output << ",";
				}
				output << "|";
			}

			dst = output.str();
			if (!dst.empty()) ret = Error::SUCCESSFUL;
		}

		return ret;
	}
}