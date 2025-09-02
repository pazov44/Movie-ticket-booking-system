#include "pch.h"
#include "models/show.h"
#include "misc.h"

namespace Entity
{
	bool Show::Update()
	{
		std::string query = "UPDATE SHOWS SET MOVIENAME = '" + movieName + "',PRICE = " + std::to_string(price) + " ,DATE = '" + date + "',CINEMANAME = '" + cinemaName + "' WHERE ID = " + std::to_string(id);

		bool ret = Misc::Update(query);

		if (!ret)
			DbgMsg("UpdateShow() : query not is ok");

		return ret;
	}
	int Show::Delete()
	{
		return Misc::Delete("%s", "SELECT MOVIENAME FROM SHOWS WHERE ID = " + std::to_string(id), "DELETE FROM SHOWS WHERE ID = " + std::to_string(id));
	}
	bool Show::IsValid()
	{
		if (!date.empty())
		{
			if (!movieName.empty())
			{
				if (price > 0)
				{
					return true;
				}
			}
		}
		return false;
	}
	int Show::Insert()
	{
		return Misc::Insert("%s %s %f %s", "INSERT INTO SHOWS(MOVIENAME,DATE,PRICE,CINEMANAME) VALUES(?,?,?,?)", &movieName, &date, price, &cinemaName);
	}
	int Show::Select(std::vector<Entity::Show>& shows)
	{
		int ret = Error::ERROR_NOT_EXISTS;

		std::string dst;
		if (Misc::Select("%d %s %f %s", "SELECT ID,DATE,PRICE,CINEMANAME FROM SHOWS WHERE MOVIENAME = '" + movieName + "'", dst) == Error::SUCCESSFUL)
		{
			DbgMsg("SelectShows() : %s", dst.c_str());

			if (!dst.empty())
			{
				std::stringstream ss(dst);
				std::string showEntry;

				while (std::getline(ss, showEntry, '|'))
				{
					std::stringstream entryStream(showEntry);
					std::string field;

					Entity::Show s;
					s.movieName = movieName;

					std::getline(entryStream, field, ','); s.id = std::stoi(field);
					std::getline(entryStream, field, ','); s.date = field;
					std::getline(entryStream, field, ','); s.price = std::stof(field);
					std::getline(entryStream, field, ','); s.cinemaName = field;

					shows.push_back(s);
				}

				ret = Error::SUCCESSFUL;
			}
		}
		return ret;
	}
	int Show::Select(std::string& dst)
	{
		int ret = Error::ERROR_NOT_EXISTS;
		if (Misc::Select("%d %s %f %s", "SELECT ID,DATE,PRICE,CINEMANAME FROM SHOWS WHERE MOVIENAME = '" + movieName + "'", dst) == Error::SUCCESSFUL)
		{
			DbgMsg("SelectShows() : %s", dst.c_str());

			if (!dst.empty())
				ret = Error::SUCCESSFUL;
		}
		return ret;
	}
	int Show::Select()
	{
		int ret = Error::ERROR_NOT_EXISTS;
		std::string dst;
		if (Misc::Select("%s %s %f", "SELECT DATE,CINEMANAME,PRICE FROM SHOWS WHERE ID = " + std::to_string(id), dst) == Error::SUCCESSFUL)
		{
			if (!dst.empty())
			{
				size_t pos1 = dst.find(',');
				if (pos1 == std::string::npos) return false;

				size_t pos2 = dst.find(',', pos1 + 1);
				if (pos2 == std::string::npos) return false;

				size_t pos3 = dst.find('|', pos2 + 1);
				if (pos3 == std::string::npos) pos3 = dst.length();

				date = dst.substr(0, pos1);
				cinemaName = dst.substr(pos1 + 1, pos2 - pos1 - 1);
				try
				{
					price = std::stof(dst.substr(pos2 + 1, pos3 - pos2 - 1));
				}
				catch (...)
				{
					return ret;
				}
				id = id;

				ret = Error::SUCCESSFUL;
			}
		}
		return ret;
	}
}