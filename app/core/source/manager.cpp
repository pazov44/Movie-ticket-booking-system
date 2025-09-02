#include "pch.h"
#include "manager.h"
namespace Manager
{
	// global db manager
	static std::shared_ptr<ManagerSQL> sql;
	// global smtp request manager
	static std::shared_ptr<ManagerSMTP> smtp;

	void ManagerSQL::Init()
	{
		printf("Connecting to database server...\n");
		if (!inst->Connect() || !inst->SetDB(dataInst.schema)) isInit = false;
		Utils::Clear();
	}
	void ManagerSMTP::Init()
	{
		if(!inst->IsValidSmtp()) isInit = false;
	}
	///////////
	std::shared_ptr<ManagerSQL> GetSQL()
	{
		return sql;
	}
	std::shared_ptr<ManagerSMTP> GetSMTP()
	{
		return smtp;
	}
	bool Init(const StruConnector& ctorInit,const StruRequest& reqInit)
	{
		bool ret = true;
		if ((sql = std::make_shared<ManagerSQL>(ctorInit)) && (smtp = std::make_shared<ManagerSMTP>(reqInit)))
		{
			sql->Init();
			smtp->Init();
			if (!sql->GetStatus()) // if managers are not init correctly
			{
				DbgMsg("managerSQL is not ok");
				ret = false;
			}
			if (!smtp->GetStatus())
			{
				DbgMsg("managerSMTP is not ok");
				ret = false;
			}
		}
		else
			ret = false;
		return ret;
	}
	void Release()
	{
		sql.reset();
		smtp.reset();
	}
}