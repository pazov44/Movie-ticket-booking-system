#include "pch.h"
#include "misc.h"
#include "main.h"
#include <iomanip>

namespace Misc
{
	void PrintStrTok(std::string op, char delim, const std::string fields[], int c_fields, const int colWidths[])
	{
		int totalWidth = 0;
		for (int i = 0; i < c_fields; ++i)
			totalWidth += colWidths[i] + 1;
		totalWidth += 1;

		std::cout << std::string(totalWidth, '-') << std::endl;

		std::cout << "|";
		for (int i = 0; i < c_fields; i++)
		{
			std::cout << std::setw(colWidths[i]) << std::left << fields[i] << "|";
		}
		std::cout << std::endl;

		std::cout << std::string(totalWidth, '-') << std::endl;

		auto PrintRow = [&](const std::string& row)
			{
				size_t start = 0, end;
				int column = 0;

				std::cout << "|";
				while ((end = row.find(',', start)) != std::string::npos && column < c_fields)
				{
					std::cout << std::setw(colWidths[column]) << std::left << row.substr(start, end - start) << "|";
					start = end + 1;
					column++;
				}
				if (column < c_fields)
				{
					std::cout << std::setw(colWidths[column]) << std::left << row.substr(start) << "|";
					column++;
				}
				for (; column < c_fields; ++column)
				{
					std::cout << std::setw(colWidths[column]) << "" << "|";
				}

				std::cout << std::endl;
			};

		size_t pos;
		while ((pos = op.find(delim)) != std::string::npos)
		{
			std::string record = op.substr(0, pos);
			PrintRow(record);
			op = op.substr(pos + 1);
		}
		if (!op.empty())
		{
			PrintRow(op);
		}

		std::cout << std::string(totalWidth, '-') << std::endl;
	}
	bool ShowAllMovies()
	{
		std::string resSet;
		Entity::Movie movie;
		if (movie.Select(resSet) == Error::ERROR_NOT_EXISTS) // get movies result set
		{
			Utils::ErrMsg("Movie does not exist");
			return false;
		}
		std::string fields[] = { "Name","Genre","Language","ReleaseYear" };

		int widthField[] = {20,20,20,15};
		Misc::PrintStrTok(resSet, '|', fields, 4,widthField);
		
		return true;
	}
	bool ShowAllShows(const std::string& movieName)
	{
		std::string dst;
		Entity::Show show;
		show.SetMovieName(movieName);
		if (show.Select(dst) == Error::ERROR_NOT_EXISTS || dst.empty())
		{
			Utils::ErrMsg("No shows are found");
			return false;
		}
		std::string fields[] = { "ID","Date","Price","CinemaName"};

		int widthField[] = {5,20,10,15};
		Misc::PrintStrTok(dst, '|', fields, 4,widthField);
		return true;
	}
	bool ShowBookings()
	{
		std::string dstBookings;
		Entity::Booking book;
		book.SetUserId(conf.currUser.GetId());

		if (book.Select(dstBookings) == Error::ERROR_NOT_EXISTS)
		{
			Utils::ErrMsg("No available bookings");
			return false;
		}
		//SHOWID,FINALPRICE,SEATX,SEATY,SEATTYPE,HALLNUMBER
		std::string fields[] = { "ShowId","Price","Seat row","Seat column","Seat type","Hall number" };
		int widthField[] = { 15,15,15,15,15,15 };
		Misc::PrintStrTok(dstBookings, '|', fields, 6, widthField);

		return true;
	}
}