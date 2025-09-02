#pragma once

typedef MySQL::Connector CTOR;
typedef SMTP::Request REQ;

namespace Manager
{
	/// @brief Holds initialization data for the MySQL connector.
	struct StruConnector
	{
		std::string host;
		std::string user;
		std::string pass;
		std::string schema;
	};

	/// @brief Holds initialization data for the SMTP request.
	struct StruRequest
	{
		Entity::User sender;
		std::string smtpAddr;
	};

	/// @brief Template base class for managing shared resource instances.
	/// @tparam T Type of instance being managed.
	/// @tparam ST Struct containing initialization data for the instance.
	template<class T, class ST>
	class Manager
	{
	protected:
		/// @brief Managed shared instance.
		std::shared_ptr<T> inst;

		/// @brief True if the manager was successfully initialized.
		bool isInit;

		/// @brief Initialization data for the managed instance.
		ST dataInst;

	public:
		/// @brief Constructs a manager with the given initialization data.
		/// @param data Initialization struct for the managed instance.
		Manager(const ST& data) : isInit(true), dataInst(data) {}

		/// @brief Gets the status of the manager (whether it's initialized).
		/// @return true If initialized correctly.
		/// @return false Otherwise.
		bool GetStatus() const { return isInit; }

		/// @brief Gets the managed instance.
		/// @return Shared pointer to the instance.
		std::shared_ptr<T> GetInstance() const { return inst; }

		/// @brief Initializes the manager and instance.
		virtual void Init() = 0;

		/// @brief Gets the initialization data used for the manager.
		/// @return Const reference to the init data struct.
		const ST& GetData() const { return dataInst; }
	};

	/// @brief Manages an instance of MySQL::Connector.
	class ManagerSQL : public Manager<CTOR, StruConnector>
	{
	public:
		/// @brief Constructs a ManagerSQL with connection data.
		/// @param data Struct containing MySQL connection data.
		ManagerSQL(const StruConnector& data) : Manager(data)
		{
			inst = std::make_shared<CTOR>(dataInst.host, dataInst.user, dataInst.pass);
		}

		/// @brief Initializes the SQL connector instance.
		void Init();
	};

	/// @brief Manages an instance of SMTP::Request.
	class ManagerSMTP : public Manager<REQ, StruRequest>
	{
	public:
		/// @brief Constructs a ManagerSMTP with request data.
		/// @param data Struct containing SMTP request data.
		ManagerSMTP(const StruRequest& data) : Manager(data)
		{
			inst = std::make_shared<REQ>(dataInst.sender, dataInst.smtpAddr);
		}

		/// @brief Initializes the SMTP request instance.
		void Init();
	};

	/// @brief Gets the global SQL manager instance.
	/// @return Shared pointer to ManagerSQL instance.
	std::shared_ptr<ManagerSQL> GetSQL();

	/// @brief Gets the global SMTP manager instance.
	/// @return Shared pointer to ManagerSMTP instance.
	std::shared_ptr<ManagerSMTP> GetSMTP();

	/// @brief Initializes global manager instances.
	/// @param ctorInit Data for MySQL connector initialization.
	/// @param reqInit Data for SMTP request initialization.
	/// @return true If all managers were initialized successfully.
	/// @return false If initialization failed.
	bool Init(const StruConnector& ctorInit, const StruRequest& reqInit);

	/// @brief Releases global manager instances.
	void Release();
}