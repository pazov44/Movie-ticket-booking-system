#pragma once
#include <string>
namespace Entity
{
    /// @brief Represents a user account in the system.
    class User
    {
        std::string email;     ///< User email address.
        std::string password;  ///< User password.
        int id;                ///< User identifier.

    public:
        /// @brief Gets the user's email address.
        /// @return Email address.
        const std::string& GetEmail() const { return email; }

        /// @brief Sets the user's email address.
        /// @param email The email to set.
        void SetEmail(const std::string& email) { this->email = email; }

        /// @brief Gets the user's password.
        /// @return Password.
        const std::string& GetPassword() const { return password; }

        /// @brief Sets the user's password.
        /// @param password The password to set.
        void SetPassword(const std::string& password) { this->password = password; }

        /// @brief Gets the user ID.
        /// @return User ID.
        int GetId() const { return id; }

        /// @brief Sets the user ID.
        /// @param id The ID to set.
        void SetId(int id) { this->id = id; }

        /// @brief Validates the user's credentials or data.
        /// @return True if valid, false otherwise.
        bool IsValid();

        /// @brief Registers a new user in the system.
        /// @return Status code (0 for success, non-zero for failure).
        int SignUp();

        /// @brief Authenticates an existing user.
        /// @return Status code (0 for success, non-zero for failure).
        int LogIn();

        /// @brief Inserts the user into the database.
        /// @return Status code (0 for success, non-zero for failure).
        int Insert();

        /// @brief Selects user information from the database by ID.
        /// @return Status code (0 for success, non-zero for failure).
        int Select();

        /// @brief Retrieves all registered user emails.
        /// @param vec Vector to store user emails.
        /// @return Status code (0 for success, non-zero for failure).
        static int SelectAllUsersEmail(std::vector<std::string>& vec);
    };
}