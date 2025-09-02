#pragma once
#include "models/user.h"
#include <mutex>

namespace SMTP
{
	/// @class Request
	/// @brief Handles SMTP email requests.
	class Request
	{
		std::string smtpAddr;
		Entity::User sender;
		mutable std::mutex mx;
	public:
		/// @brief Sets the sender email and app password.
		/// @param sender Sender user credentials.
		void SetSender(const Entity::User& sender);

		/// @brief Sets the SMTP server address.
		/// @param smtpAddr SMTP server address.
		void SetServer(const std::string& smtpAddr);

		/// @brief Sends an email to all specified receivers.
		/// @param receiversEmail Vector of receiver email addresses.
		/// @param subject Email subject.
		/// @param body Email body content.
		void Send(const std::vector<std::string>& receiversEmail, const std::string& subject, const std::string& body) const;

		/// @brief Sends an email asynchronously to all specified receivers.
		/// @param receiversEmail Vector of receiver email addresses.
		/// @param subject Email subject.
		/// @param body Email body content.
		void SendAsync(const std::vector<std::string>& receiversEmail, const std::string& subject, const std::string& body) const;

		/// @brief Constructs a Request with given sender and SMTP server address.
		/// @param sender Sender user credentials.
		/// @param smtpAddr SMTP server address.
		Request(const Entity::User& sender, const std::string& smtpAddr);

		/// @brief Default constructor.
		Request() = default;

		bool IsValidSmtp();
	};

	/// @brief Sends a notification email to users.
	/// @param subject Subject of the email.
	/// @param msg Body message of the email.
	/// @param emailList List of emails to send to; if empty, sends to all users.
	void NotifyUsers(const std::string& subject, const std::string& msg, std::vector<std::string> emailList = {});
}