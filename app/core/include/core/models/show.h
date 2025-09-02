#pragma once
#include <string>
namespace Entity
{
    /// @brief Represents a movie show/display information.
    class Show
    {
        std::string movieName;   ///< Name of the movie.
        std::string date;        ///< Show date and time in "YYYY-MM-DD HH:MM:SS" format.
        float price;             ///< Ticket price.
        std::string cinemaName;  ///< Cinema name where the show takes place.
        int id;                  ///< Show identifier.

    public:
        /// @brief Default constructor initializing price to 0.
        Show() : price(0) {}

        /// @brief Gets the movie name.
        /// @return Movie name.
        const std::string& GetMovieName() const { return movieName; }

        /// @brief Sets the movie name.
        /// @param movieName The name to set.
        void SetMovieName(const std::string& movieName) { this->movieName = movieName; }

        /// @brief Gets the show date.
        /// @return Date string.
        const std::string& GetDate() const { return date; }

        /// @brief Sets the show date.
        /// @param date Date string in "YYYY-MM-DD HH:MM:SS" format.
        void SetDate(const std::string& date) { this->date = date; }

        /// @brief Gets the ticket price.
        /// @return Price of the ticket.
        float GetPrice() const { return price; }

        /// @brief Sets the ticket price.
        /// @param price Price to set.
        void SetPrice(float price) { this->price = price; }

        /// @brief Gets the cinema name.
        /// @return Cinema name.
        const std::string& GetCinemaName() const { return cinemaName; }

        /// @brief Sets the cinema name.
        /// @param cinemaName The cinema name to set.
        void SetCinemaName(const std::string& cinemaName) { this->cinemaName = cinemaName; }

        /// @brief Gets the show ID.
        /// @return Show identifier.
        int GetId() const { return id; }

        /// @brief Sets the show ID.
        /// @param id The identifier to set.
        void SetId(int id) { this->id = id; }

        /// @brief Validates the show data.
        /// @return True if the data is valid, false otherwise.
        bool IsValid();

        /// @brief Updates the show information in the database.
        /// @return True on success, false on failure.
        bool Update();

        /// @brief Deletes the show from the database.
        /// @return Status code (0 for success, non-zero for failure).
        int Delete();

        /// @brief Inserts the show into the database.
        /// @return Status code (0 for success, non-zero for failure).
        int Insert();

        /// @brief Selects all shows and stores them in a vector.
        /// @param shows Vector to store the selected shows.
        /// @return Status code (0 for success, non-zero for failure).
        int Select(std::vector<Entity::Show>& shows);

        /// @brief Selects all shows and stores them in a string.
        /// @param dst String to store the selected shows.
        /// @return Status code (0 for success, non-zero for failure).
        int Select(std::string& dst);

        /// @brief Selects a show by ID.
        /// @return Status code (0 for success, non-zero for failure).
        int Select();
    };
}