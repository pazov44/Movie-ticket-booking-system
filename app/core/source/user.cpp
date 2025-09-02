#include "pch.h"
#include "models/user.h"
#include "misc.h"
namespace Entity
{
	bool User::IsValid()
	{
		std::regex patternEmail(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
		std::regex patternPass(R"(^[a-zA-Z0-9]+$)");

		return std::regex_match(email, patternEmail) && std::regex_match(password, patternPass) && password.size() >= 8;
	}
	int User::SignUp()
	{
		if (LogIn() != Error::ERROR_NOT_EXISTS) return Error::ERROR_EXISTS;
		if (!IsValid()) return Error::ERROR_INPUT;
		return Insert();
	}
	int User::LogIn()
	{
		return Select(); // return select db operation result
	}
	int User::Insert()
	{
		std::string hpass; // hashed passwword

		byte salt[SALT_BYTES];
		Crypt::GenRandom(salt, SALT_BYTES);

		Crypt::CalcHash(password, salt, SALT_BYTES, hpass); // generate password hash combined with salt

		std::string hexSalt;
		Crypt::ByteToHex(salt, SALT_BYTES, hexSalt);

		return Misc::Insert("%s %s %s", "INSERT INTO ACCOUNTS(EMAIL,PASSWORD,SALT) VALUES(?,?,?)", &email, &hpass, &hexSalt);
	}
	int User::Select()
	{
		std::string retrievedSalt;
		if (Misc::Select("%s", "SELECT SALT FROM ACCOUNTS WHERE EMAIL = '" +email + "'", retrievedSalt) != Error::SUCCESSFUL) return Error::ERROR_NOT_EXISTS;
		Utils::Trim(retrievedSalt, "|,", false); // remove unnecessary chars from salt

		byte salt[SALT_BYTES];
		Crypt::HexToByte(retrievedSalt, SALT_BYTES, salt);

		std::string hpass;
		Crypt::CalcHash(password, salt, SALT_BYTES, hpass);

		std::string retrievedPass;

		int ret = Misc::Select("%s", "SELECT PASSWORD FROM ACCOUNTS WHERE EMAIL = '" + email + "'", retrievedPass);
		if (ret != Error::ERROR_NOT_EXISTS)
		{
			Utils::Trim(retrievedPass, "|", false); // remove last pipe appended from Read fn

			DbgMsg("SelectUserExist() : %s %s", email.c_str(), retrievedPass.c_str());
			ret = Error::ERROR_INPUT; // if email is found but password is wrong, return error_input

			if (retrievedPass == hpass)
			{
				std::string idDst;
				if (Misc::Select("%d", "SELECT ID FROM ACCOUNTS WHERE EMAIL = '" + email + "'", idDst) != Error::ERROR_NOT_EXISTS)
				{
					try
					{
						id = std::stod(idDst); // assign user id
						DbgMsg("SelectUserExist() : %d", id);
						ret = Error::SUCCESSFUL;
					}
					catch (...)
					{
						DbgMsg("error SelectUserExist() id cannot read");
						ret = Error::ERROR_FAILED;
					}
				}
			}
		}
		return ret;
	}
	int User::SelectAllUsersEmail(std::vector<std::string>& vec)
	{
		std::string dst;
		int ret = Error::ERROR_FAILED;

		if (Misc::Select("%s", "SELECT EMAIL FROM ACCOUNTS WHERE ID > 1 ORDER BY ID ASC", dst) == Error::SUCCESSFUL)
		{
			size_t from = 0, to;

			while ((to = dst.find('|', from)) != std::string::npos)
			{
				std::string email = dst.substr(from, to - from);
				if (!email.empty())
					vec.push_back(email);

				from = to + 1;
			}

			std::string last = dst.substr(from);
			if (!last.empty())
				vec.push_back(last);

			for (const auto& email : vec)
				DbgMsg("SelectAllUsersEmail() : %s", email.c_str());

			if (!vec.empty())
				ret = Error::SUCCESSFUL;
		}
		return ret;
	}
}