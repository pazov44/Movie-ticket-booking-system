#pragma once

/// @def DEFAULT_POINT
/// @brief Default value for point coordinates x and y.
#define DEFAULT_POINT -1

namespace Matrix
{
    /// @class Sparse
    /// @brief Represents a sparse matrix storing string values at specific coordinates.
    class Sparse
    {
        /// @struct Element
        /// @brief Represents a single element in the sparse matrix.
        struct Element
        {
            int x;           ///< X coordinate of the element.
            int y;           ///< Y coordinate of the element.
            std::string val; ///< Value stored at the element.
        };

        Element* data;       ///< Pointer to dynamic array of matrix elements.
        std::string mvalue;  ///< Default value of the matrix for unset positions.

        int size;            ///< Total allocated size of the elements array.
        int count;           ///< Current number of used elements.

        int rowSize;         ///< Number of rows in the matrix.
        int colSize;         ///< Number of columns in the matrix.

        /// @brief Prints the matrix with given dimensions.
        /// @param size1 First dimension size (rows or columns).
        /// @param size2 Second dimension size (columns or rows).
        void Print(int size1, int size2) const;

    public:
        /// @brief Constructs a Sparse matrix with specified size and default value.
        /// @param sizeCol Number of columns.
        /// @param sizeRow Number of rows.
        /// @param mvalue Default value for empty elements.
        Sparse(int sizeCol, int sizeRow, std::string mvalue);

        /// @brief Default constructor initializing a 5x5 matrix with default value "x".
        Sparse() : Sparse(5, 5, "x") {}

        /// @brief Destructor, releases allocated resources.
        ~Sparse();

        /// @brief Gets the value stored at the specified coordinates.
        /// @param x X coordinate.
        /// @param y Y coordinate.
        /// @return Const reference to the stored value or default value if not set.
        const std::string& Get(int x, int y) const;

        /// @brief Finds the coordinates of a given value in the matrix.
        /// @param val Value to search for.
        /// @return POINT struct with x and y coordinates; DEFAULT_POINT if not found.
        POINT Get(const std::string& val) const;

        /// @brief Sets a value at the specified coordinates in the matrix.
        /// @param x X coordinate.
        /// @param y Y coordinate.
        /// @param val Value to set.
        /// @return true if the value was set successfully, false otherwise.
        bool Set(int x, int y, const std::string& val);

        /// @brief Prints the matrix row by row.
        void PrintRows()
        {
            Print(rowSize, colSize);
        }

        /// @brief Prints the matrix column by column.
        void PrintCols()
        {
            Print(colSize, rowSize);
        }
    };
}