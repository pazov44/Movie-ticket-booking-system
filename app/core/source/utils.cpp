#include "pch.h"
#include "utils.h"

namespace Utils
{
	void Exit()
	{
		ErrMsg("Quiting!");
		ExitProcess(0);
	}
	void ErrMsg(const std::string& msg, int sec)
	{
		if (sec <= 0) sec = 1;

		Clear();
		std::cout << msg << std::endl;
		Sleep(sec * 1000);
		Clear();
	}
	void Clear()
	{
		system("cls"); 
	}
	void Trim(std::string& src,const std::string& chars, bool possible)
	{
		std::string buff;

		for (const auto& c : src)
		{
			bool isIn = chars.find(c) != std::string::npos;

			if ((possible && isIn) || (!possible && !isIn))
			{
				buff.push_back(c);
			}
		}
		src = buff;
	}
}