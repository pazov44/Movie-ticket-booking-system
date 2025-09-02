#pragma once

namespace Options
{
	// authentication option
	int LogSign(int mode);
	// admin option
	void InsertMovie();
	// admin option
	void DeleteMovie();
	// admin option
	void InsertShow();
	//admin option
	void DeleteShow();
	// admin option
	void UpdateShow();
	// customer option
	void BookMovie();
	//customer option
	void CancelBooking();
}