#pragma once
#include <string>
#include "config.h"
#include "core\type.h"

struct Config
{
	// db credentials
	std::string dbuser;
	std::string dbpass;
	std::string dbhost;
	// app admin username
	std::string ademail;

	// main database schema in mysql server
	std::string schema;

	// app smtp sender user
	Entity::User sender;
	// app smtp server
	std::string smtpServer;

	//current user data
	Entity::User currUser;

};
extern Config conf; // global app config


