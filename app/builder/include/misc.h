#pragma once

namespace Misc
{
	// enter a non negative number into a str
	void EnterNumber(std::string& num, bool floating = 0);
	// enter movie struct data
	void EnterMovieData(Entity::Movie& movie);
	//// enter movie cinema name
	//void EnterMovieCinema(Entity::Movie& movie);
	void EnterDateTime(Entity::Show& show);
	// enter show struct data
	bool EnterShowData(Entity::Show& show);
	// enter user's password securly
	void EnterUserPassword(Entity::User& user);
	// print string with delimeter in table format
	void PrintStrTok(std::string op, char delim, const std::string fields[], int c_fields, const int colWidths[]);
	// print all movies in a table-like format
	//returns false if no movies exist
	bool ShowAllMovies();
	// print all shows of a movie
	// returns false if no such movie exists
	bool ShowAllShows(const std::string& movieName);
	// show all bookings of current user
	bool ShowBookings();
	void EnterShowCinema(Entity::Show& show);
	void EnterBookingData(Entity::Booking& book, const Entity::Show& show);
	bool ChooseMovieShow(Entity::Show& show);
}