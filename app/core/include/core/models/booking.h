#pragma once
#include <string>

namespace Entity
{
    /// @brief Represents a customer booking in the system.
    class Booking
    {
        int showId;             ///< Associated show identifier.
        int userId;             ///< Associated user identifier.
        int seatX;              ///< Seat row coordinate.
        int seatY;              ///< Seat column coordinate.
        float finalPrice;       ///< Price with seat tax included.
        int hallNumber;         ///< Hall number where the booking is made.
        std::string seatType;   ///< Type of seat booked.

    public:
        /// @brief Default constructor initializing numeric members to zero.
        Booking() : finalPrice(0), seatX(0), seatY(0), showId(0), hallNumber(0) {}

        /// @brief Gets the show ID.
        int GetShowId() const { return showId; }

        /// @brief Sets the show ID.
        void SetShowId(int showId) { this->showId = showId; }

        /// @brief Gets the user ID.
        int GetUserId() const { return userId; }

        /// @brief Sets the user ID.
        void SetUserId(int userId) { this->userId = userId; }

        /// @brief Gets the seat's X coordinate (row).
        int GetSeatX() const { return seatX; }

        /// @brief Sets the seat's X coordinate (row).
        void SetSeatX(int seatX) { this->seatX = seatX;; }

        /// @brief Gets the seat's Y coordinate (column).
        int GetSeatY() const { return seatY; }

        /// @brief Sets the seat's Y coordinate (column).
        void SetSeatY(int seatY) { this->seatY = seatY; }

        /// @brief Gets the final ticket price.
        float GetFinalPrice() const { return finalPrice; }

        /// @brief Sets the final ticket price.
        void SetFinalPrice(float finalPrice) { this->finalPrice = finalPrice; }

        /// @brief Gets the hall number.
        int GetHallNumber() const { return hallNumber; }

        /// @brief Sets the hall number.
        void SetHallNumber(int hallNumber) { this->hallNumber = hallNumber; }

        /// @brief Gets the seat type.
        const std::string& GetSeatType() const { return seatType; }

        /// @brief Sets the seat type.
        void SetSeatType(const std::string& seatType) { this->seatType = seatType; }

        /// @brief Deletes the booking from the database.
        /// @return Status code (0 for success, non-zero for failure).
        int Delete();

        /// @brief Inserts the booking into the database.
        /// @return Status code (0 for success, non-zero for failure).
        int Insert();

        /// @brief Selects booking data from the database into a string.
        /// @param dst Destination string to hold booking data.
        /// @return Status code (0 for success, non-zero for failure).
        int Select(std::string& dst);

        /// @brief Selects multiple booking records into a vector.
        /// @param bookings Destination vector to hold multiple bookings.
        /// @return Status code (0 for success, non-zero for failure).
        int Select(std::vector<Entity::Booking>& bookings);
    };
}