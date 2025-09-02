#include "pch.h"
#include "models/movie.h"
#include "misc.h"

namespace Entity
{
	int Movie::Delete()
	{
		return Misc::Delete("%s", "SELECT NAME AS MovieName FROM MOVIES WHERE NAME = '" + name + "' "
			"UNION "
			"SELECT MOVIENAME AS MovieName FROM SHOWS WHERE MOVIENAME = '" + name + "'", "DELETE FROM MOVIES WHERE NAME = '" + name + '\'', true);
	}
	int Movie::Insert()
	{
		return Misc::Insert("%s %s %s %d", "INSERT INTO MOVIES(NAME,GENRE,LANGUAGE,RELEASEYEAR) VALUES(?,?,?,?)", &name, &genre, &language, releaseYear);
	}
	bool Movie::IsValid()
	{
		if (releaseYear >= 1888 && releaseYear <= 2025)
		{
			if (!genre.empty())
			{
				if (!name.empty())
				{
					return true; // if all criteria is met
				}
			}
		}
		return false;
	}
	int Movie::Select(std::string& dst)
	{
		int ret = Error::ERROR_NOT_EXISTS;
		if (Misc::Select("%s %s %s %d", name.empty() ? "SELECT NAME,GENRE,LANGUAGE,RELEASEYEAR FROM MOVIES" : "SELECT NAME,GENRE,LANGUAGE,RELEASEYEAR FROM MOVIES WHERE NAME = '" + name + '\'', dst) == Error::SUCCESSFUL)
		{
			DbgMsg("SelectMovie() : %s", dst.c_str());

			if (!dst.empty())
				ret = Error::SUCCESSFUL;
		}
		return ret;
	}
}