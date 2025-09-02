#pragma once
#include <string>
namespace Entity
{
    /// @brief Represents movie main information.
    class Movie
    {
        std::string name;        ///< Movie name.
        std::string genre;       ///< Movie genre.
        int releaseYear;         ///< Year the movie was released.
        std::string language;    ///< Language of the movie.

    public:
        /// @brief Default constructor initializing releaseYear to 1888.
        Movie() : releaseYear(1888) {}

        /// @brief Gets the movie name.
        /// @return Movie name.
        const std::string& GetName() const { return name; }

        /// @brief Sets the movie name.
        /// @param name The name to set.
        void SetName(const std::string& name) { this->name = name; }

        /// @brief Gets the movie genre.
        /// @return Movie genre.
        const std::string& GetGenre() const { return genre; }

        /// @brief Sets the movie genre.
        /// @param genre The genre to set.
        void SetGenre(const std::string& genre) { this->genre = genre; }

        /// @brief Gets the release year of the movie.
        /// @return Release year.
        int GetReleaseYear() const { return releaseYear; }

        /// @brief Sets the release year of the movie.
        /// @param releaseYear The year to set.
        void SetReleaseYear(int releaseYear) { this->releaseYear = releaseYear; }

        /// @brief Gets the language of the movie.
        /// @return Movie language.
        const std::string& GetLanguage() const { return language; }

        /// @brief Sets the language of the movie.
        /// @param language The language to set.
        void SetLanguage(const std::string& language) { this->language = language; }

        /// @brief Checks if the movie data is valid.
        /// @return True if valid, false otherwise.
        bool IsValid();

        /// @brief Deletes the movie from the database.
        /// @return Status code (0 for success, non-zero for failure).
        int Delete();

        /// @brief Inserts the movie into the database.
        /// @return Status code (0 for success, non-zero for failure).
        int Insert();

        /// @brief Selects movie data from the database.
        /// @param dst Output string to store selected data.
        /// @return Status code (0 for success, non-zero for failure).
        int Select(std::string& dst);
    };
}