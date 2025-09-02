#include "pch.h"
#include "menu.h"
#include "core\database.h"
#include "main.h"
#include "core\manager.h"
Config conf;

static bool Init();
int main()
{
	if (!Init()) // if startup fails
	{
		Utils::ErrMsg("Internal Error! Please try again later.");
		Utils::Exit();
		return 1;
	}
	for(;;)
	switch (Menu::AuthMenu())
	{
	case Menu::ENTER_ADMIN: Menu::AdminMenu(); break;
	case Menu::ENTER_CUSTOMER: Menu::CustomerMenu(); break;
	}
}
bool Init()
{
	Manager::StruConnector sqlInit;
	Manager::StruRequest smtpInit;

	std::ifstream file(ENV_FILE);
	if (!file.is_open())
	{
		return false;
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '#')
			continue;

		size_t pos = line.find('=');
		if (pos == std::string::npos)
			continue;

		std::string name = line.substr(0, pos);
		std::string val = line.substr(pos + 1);

		std::string env = name + '=' + val;//name=value

		if (putenv(env.c_str()))
		{
			return false;
		}
	}

	char* env[] = 
	{
		getenv("hostAZ"),
		getenv("userAZ"),
		getenv("passAZ"),
		getenv("emailSMTP"),
		getenv("passSMTP"),
		getenv("serverSMTP"),
		getenv("schemaAZ")
	};
	conf.dbhost = sqlInit.host = env[0] ? env[0] : "";
	conf.dbuser = sqlInit.user = env[1] ? env[1] : "";
	conf.dbpass = sqlInit.pass = env[2] ? env[2] : "";

	smtpInit.sender.SetEmail(env[3] ? env[3] : "");
	smtpInit.sender.SetPassword(env[4] ? env[4] : "");
	conf.sender = smtpInit.sender;

	conf.smtpServer = smtpInit.smtpAddr = env[5] ? env[5] : "";
	conf.schema = sqlInit.schema = env[6] ? env[6] : "";
	
	conf.ademail = DB_ADMIN_EMAIL;
	
	return Manager::Init(sqlInit, smtpInit);
}