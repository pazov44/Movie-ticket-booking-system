#include "pch.h"
#include "misc.h"
#include <conio.h>
#include "main.h"
#include "core\matrix.h"
namespace Misc
{
	void EnterNumber(std::string& num, bool floating)
	{
		for (;;)
		{
			std::cout << ":";
			std::cin >> num;
			try
			{
				size_t pos;

				if (floating)
				{
					float val = std::stof(num, &pos);
					if (pos != num.length() || val < 0)
						throw std::runtime_error("error");
				}
				else
				{
					int val = std::stoi(num, &pos);
					if (pos != num.length() || val < 0)
						throw std::runtime_error("error");
				}

				break;
			}
			catch (...)
			{
			}
		}
		Utils::Clear();
	}
	void EnterDateTime(Entity::Show& show)
	{
		std::regex pattern(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$)");
		std::string date;
		do
		{
			std::cout << ':';
			
			std::getline(std::cin, date);
		} while (!std::regex_match(date, pattern));
		show.SetDate(date);
		Utils::Clear();
	}
	void EnterMovieData(Entity::Movie& movie)
	{
		std::cout << "Enter movie's name\n:";
		std::string name;
		std::cin >> name;
		movie.SetName(name);

		Utils::Clear();
		std::cout << "Enter movie's genre\n:";
		std::string genre;
		std::cin >> genre;
		movie.SetGenre(genre);

		Utils::Clear();

		std::cout << "Enter movie's release year\n";
		std::string num;
		EnterNumber(num);

		std::cout << "Enter movie's language\n:";
		std::string language;
		std::cin >> language;
		movie.SetLanguage(language);

		movie.SetReleaseYear(std::stoi(num));
		Utils::Clear();
	}
	bool EnterShowData(Entity::Show& show)
	{
		std::cout << "Enter movie's name\n:";
		std::string movieName;
		std::cin >> movieName;
		show.SetMovieName(movieName);

		Utils::Clear();
		std::string dstData;
		Entity::Movie movTmp;
		movTmp.SetName(movieName);

		if (movTmp.Select(dstData) == Error::ERROR_NOT_EXISTS)
		{
			Utils::ErrMsg("Movie does not exist!"); // if movie doesn't exists, cannot add show for it
			return false;
		}
		std::cout << "Enter show date in YYYY-MM-DD HH:MM:SS format\n";
		getchar();

		EnterDateTime(show);

		Utils::Clear();

		std::cout << "Enter price\n";
		std::string price;
		EnterNumber(price, true);
		show.SetPrice(std::stof(price));

		Utils::Clear();

		EnterShowCinema(show);

		Utils::Clear();
		return true;
	}
	void EnterUserPassword(Entity::User& user)
	{
		std::string pass;;
		char c;

		std::cout << "\nEnter password\n:";

		for (;;)
		{
			c = _getch();

			if (c == 13) // enter
				break;
			if (c == 8) //back
			{
				if (!pass.empty())
				{
					pass.pop_back();
					std::cout << "\b \b";
				}
			}
			else if (isprint(c)) // if is num,symbol,letter
			{
				pass.push_back(c);
				std::cout << '*';
			}
		}
		user.SetPassword(pass);
	}
	static int EnterSeat(const std::vector<Entity::Booking>& lsBookings, Entity::Booking& book);

	static void EnterPaymentData();
	void EnterBookingData(Entity::Booking& book, const Entity::Show& show)
	{
		std::cout << "Enter seat type number:\n1. Silver\n2. Gold\n3. Platinum\n";
		std::string choice;
		do
		{
			std::cout << ":";
			std::cin >> choice;
		} while (choice != "1" && choice != "2" && choice != "3");
		book.SetSeatType(seatType.at((Seat)(std::stod(choice)))); //assign seat type

		Utils::Clear();

		book.SetFinalPrice(show.GetPrice() + seatPrice.at((Seat)std::stod(choice))); //final price with added seat price

		Utils::Clear();

		std::string hallNumber;
		do
		{
			std::cout << "Enter hall number:\n1. Hall 1\n2. Hall 2\n3. Hall 3\n4. Hall 4\n";
			Misc::EnterNumber(hallNumber);
		} while (std::stod(hallNumber) > HALLS);
		book.SetHallNumber(std::stod(hallNumber));

		std::vector<Entity::Booking> bookings;
		if (book.Select(bookings) != Error::ERROR_FAILED)
		{
			int seatNum = EnterSeat(bookings, book);

			EnterPaymentData();
			if (book.Insert() == Error::SUCCESSFUL)
			{
				Utils::ErrMsg("Show booked successfuly!");
				std::string msg =
					"You have made a booking\n"
					"Price: " + std::to_string(book.GetFinalPrice()).substr(0, 4) + "\n"
					"Hall: " + std::to_string(book.GetHallNumber()) + "\n"
					"Seat type: " + book.GetSeatType() + "\n"
					"Seat row: " + std::to_string(book.GetSeatX()) + "\n"
					"Seat column: " + std::to_string(book.GetSeatY()) + "\n"
					"Seat number: " + std::to_string(seatNum) + "\n"
					"Show ID: " + std::to_string(book.GetShowId()) + "\n";

				SMTP::NotifyUsers("New booking", msg, { conf.currUser.GetEmail()});
			}
			else
				Utils::ErrMsg("Unexpected error while making booking. Please try again later.");
		}
		else
		{
			Utils::ErrMsg("Unexpected error getting booking data. Please try again later.");
		}
	}

	int EnterSeat(const std::vector<Entity::Booking>& lsBookings, Entity::Booking& book)
	{
		Matrix::Sparse seats(ROW_SIZE, COL_SIZE, "x"); //seats in hall
		int nSeat = 1;

		for (int i = 0; i < COL_SIZE; i++) // set all seats initialy to numeric values
		{
			for (int j = 0; j < ROW_SIZE; j++)
			{
				seats.Set(i,j, std::to_string(nSeat));
				nSeat++;
			}
		}
		for (const auto& book : lsBookings) //mark booked seats at chosen hall as x
		{
			seats.Set(book.GetSeatX(), book.GetSeatY(), "x");
		}

		for (;;)
		{
			std::string seatVal;
			do
			{
				std::cout << "\nSeats seen as 'x' are booked\n\n";
				seats.PrintRows();

				std::cout << "\nEnter seat number:\n";
				Misc::EnterNumber(seatVal);
			} while (std::stod(seatVal) > ROW_SIZE * COL_SIZE);

			POINT p = { DEFAULT_POINT,DEFAULT_POINT }; //default pos
			POINT p2 = seats.Get(seatVal); // entered seat pos
			if (p2.x == p.x && p2.y == p.y)
			{
				Utils::ErrMsg("Seat is booked, try again!");
			}
			else
			{
				book.SetSeatX(p2.x);
				book.SetSeatY(p2.y);
				return std::stod(seatVal);
			}
		}
	}
	void EnterShowCinema(Entity::Show& show)
	{
		std::cout << "Choose show's cinema:\n1. CinemaBurgas\n2. CinemaVarna\n";
		std::string choice;
		do
		{
			std::cout << ':';
			std::cin >> choice;
		} while (choice != "1" && choice != "2");

		show.SetCinemaName(choice == "1" ? "CinemaBurgas" : "CinemaVarna");
		Utils::Clear();
	}
	void EnterPaymentData()
	{
		std::cout << "Enter payment info:\n";
		Sleep(1000);
		Utils::Clear();

		std::string input;
		for (int i = 0; i < 2; i++)
		{
			std::cout << "Enter placeholder " << (i == 0 ? "first name:\n" : "last name:\n");
			do
			{
				std::cout << ":";
				std::cin >> input;
			} while (!std::all_of(input.begin(), input.end(), [](int c) {return isalpha(c); }));
			Utils::Clear();
		}
		std::cout << "Enter credit card number in XXXX-XXXX-XXXX-XXXX format:\n";
		do
		{
			std::cout << ":";
			std::cin >> input;
			Utils::Trim(input, "-", false);

		} while (!Misc::LuhnCheck(input));
		Utils::Clear();

		std::regex patternExpDate(R"(^(0[1-9]|1[0-2])\/\d{2}$)");
		std::cout << "Enter expiration date in MM/YY format:\n";
		do
		{
			std::cout << ":";
			std::cin >> input;
		} while (!std::regex_match(input, patternExpDate));
		Utils::Clear();

		std::cout << "Enter cvv in XXX format:\n";
		do
		{
			std::cout << ":";
			std::cin >> input;
		} while (input.size() != 3 || !std::all_of(input.begin(), input.end(), [](int c) {return isdigit(c); }));
		Utils::Clear();
	}
	bool ChooseMovieShow(Entity::Show& show)
	{
		if (!Misc::ShowAllMovies()) return false;
		std::cout << "\n\nEnter movie's name to book\n:";
		std::string movieName;
		std::cin >> movieName;

		Utils::Clear();
		if (!Misc::ShowAllShows(movieName)) return false;

		std::cout << "Enter show's id to book:\n";
		std::string choice;// id of chosen show
		Misc::EnterNumber(choice);

		show.SetId(std::stod(choice));
		Utils::Clear();

		return show.Select() == Error::SUCCESSFUL;
	}
}