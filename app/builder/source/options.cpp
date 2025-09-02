#include "pch.h"
#include "options.h"
#include "menu.h"
#include "misc.h"
#include "core\smtp.h"
namespace Options
{	
	int LogSign(int mode)
	{	
		Entity::User user; // user data

		std::cout << "\nEnter email\n:";
		std::string email;
		std::cin >> email;
		user.SetEmail(email);

		Misc::EnterUserPassword(user);

		int res = mode == 1 ? user.LogIn() : user.SignUp();
		if (res != Error::SUCCESSFUL) // if logging or signing is not successful
		{
			switch (res)
			{
			case Error::ERROR_NOT_EXISTS: Utils::ErrMsg("User does not exist!"); break;
			case Error::ERROR_INPUT: mode == 1 ? Utils::ErrMsg("Invalid password!") : Utils::ErrMsg("Email needs to follow this example:\n\"example@gmail.com\"\nPassword needs to contain:\n1 small letter\n1 big letter\n1 number", 5); break;
			case Error::ERROR_EXISTS: Utils::ErrMsg("User already exists."); break;
			default: Utils::ErrMsg("Unexpected error! Try again later."); break;
			}
			return res;
		}
		conf.currUser = user; // assign current user data to config

		return stricmp(user.GetEmail().c_str(), conf.ademail.c_str()) == 0 ? Menu::ENTER_ADMIN : Menu::ENTER_CUSTOMER;
	}
	void InsertMovie()
	{
		Entity::Movie add;
		Misc::EnterMovieData(add);

		int res = Error::ERROR_FAILED;
		if (add.IsValid())
		{
			std::string dstData;
			if (add.Select(dstData) == Error::ERROR_NOT_EXISTS)
			{
				res = add.Insert();
			}
			else
				res = ERROR_EXISTS;
		}
		else
		{
			Utils::ErrMsg("Incorrect movie data!");
		}

		if (res == Error::ERROR_EXISTS)
		{
			Utils::ErrMsg("Movie already exists!");
		}
		else if(res == Error::SUCCESSFUL)
		{
			Utils::ErrMsg("Movie has been added successfully!");

			std::string msg =
				"A new movie has been released!\n\n"
				"Name: " + add.GetName() + "\n"
				"Genre: " + add.GetGenre() + "\n"
				"Language: " + add.GetLanguage() + "\n"
				"Release year: " + std::to_string(add.GetReleaseYear()) + "\n";
			SMTP::NotifyUsers("New movie", msg); // notify all users of newly inserted movie
		}
	}
	void DeleteMovie()
	{
		if(!Misc::ShowAllMovies()) return;

		Entity::Movie add;
		std::cout << "\n\nEnter movie's name\n:";
		std::string name;
		std::cin >> name;
		add.SetName(name);
		
		switch (add.Delete())
		{
		case Error::SUCCESSFUL: Utils::ErrMsg("Successfully deleted!"); break;
		case Error::ERROR_FAILED: Utils::ErrMsg("Error! Movie cannot be deleted.");; break;
		}
	}
	void InsertShow()
	{
		if(!Misc::ShowAllMovies()) return; // print movies so admin can see names
		std::cout << "\n\n";

		Entity::Show add;
		if (Misc::EnterShowData(add))
		{
			std::vector<Entity::Show> shows;
			if (add.Select(shows) == Error::SUCCESSFUL) // check if show exists before inserting
			{
				for (const auto& show : shows)
				{
					if (show.GetDate() == add.GetDate() && show.GetCinemaName() == add.GetCinemaName())
					{
						Utils::ErrMsg("Show already exists!");
						return;
					}
				}
			}
			switch (add.Insert())
			{
			case Error::SUCCESSFUL: Utils::ErrMsg("Successfully inserted!"); break;
			case Error::ERROR_EXISTS: Utils::ErrMsg("Show already exists!"); break;
			default: Utils::ErrMsg("Unexpected error. Try later."); break;
			}
		}
	}
	void DeleteShow()
	{
		if(!Misc::ShowAllMovies()) return;
		std::cout << "\n\nEnter movie's name\n:";
		std::string movieName;
		std::cin >> movieName;

		std::string dstData;
		Entity::Movie mov;
		mov.SetName(movieName);

		if (mov.Select(dstData) != Error::SUCCESSFUL)
		{
			Utils::ErrMsg("Movie does not exist!"); // check if movie exists
			return;
		}

		Utils::Clear();
		if (!Misc::ShowAllShows(movieName))
		{
			return; // stop if now shows are available
		}

		std::cout << "Enter show's ID\n";
		std::string id;
		Misc::EnterNumber(id);
		
		Entity::Show del;
		del.SetId(std::stoi(id));
		switch (del.Delete())
		{
		case Error::SUCCESSFUL: Utils::ErrMsg("Successfully deleted!"); break;
		case Error::ERROR_FAILED: Utils::ErrMsg("Show not found!"); break;
		}
	}
	void UpdateShow()
	{
		if(!Misc::ShowAllMovies()) return;

		std::cout << "\n\nEnter movie's name\n:";
		std::string movieName;
		std::cin >> movieName;
		
		Utils::Clear();
		if (!Misc::ShowAllShows(movieName)) // if no shows for entered movie, stop func
		{
			return;
		}
		Entity::Show newShow;

		std::cout << "\nEnter show's ID\n";
		std::string id;
		Misc::EnterNumber(id);
		newShow.SetId(std::stoi(id));

		newShow.SetMovieName(movieName); //assign entered movie name

		std::cout << "Enter show's new date in YYYY-MM-DD HH:MM:SS format\n";
		Misc::EnterDateTime(newShow);
		Utils::Clear();

		std::cout << "Enter show's new price\n";
		std::string price;
		Misc::EnterNumber(price, true);
		newShow.SetPrice(std::stof(price));

		Utils::Clear();
		Misc::EnterShowCinema(newShow);

		newShow.Update() ? Utils::ErrMsg("Successfully updated!") : Utils::ErrMsg("Update failed!");
	}
	void BookMovie()
	{
		Entity::Show chosenShow;
		if (!Misc::ChooseMovieShow(chosenShow))
		{
			return;
		}

		Entity::Booking book;
		book.SetShowId(chosenShow.GetId());
		book.SetUserId(conf.currUser.GetId());

		Misc::EnterBookingData(book,chosenShow);
	}
	void CancelBooking()
	{
		if (!Misc::ShowBookings()) return;
		
		std::cout << "\n\nEnter the following data to cancel:\n";
		std::string x, y;
		std::cout << "\n\nEnter seat row:\n";
		Misc::EnterNumber(x);

		Misc::ShowBookings();

		std::cout << "\n\nEnter the following data to cancel:\n";
		std::cout << "\n\nEnter seat column:\n";
		Misc::EnterNumber(y);

		Entity::Booking book;
		book.SetSeatX(std::stod(x));
		book.SetSeatY(std::stod(y));

		if (book.Delete() == Error::SUCCESSFUL)
		{
			Utils::ErrMsg("Canceled successfully");
			std::string msg =
				"You have canceled a booking\n"
				"Seat column: " + x + "\n"
				"Seat row: " + y + "\n";
			SMTP::NotifyUsers("Booking cancel", msg, { conf.currUser.GetEmail()});
		}
		else
		{
			Utils::ErrMsg("Internal error, please try again later.");
		}
	}
}
