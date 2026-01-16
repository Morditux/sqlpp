/* 
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014-2015 Morditux
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * File:   Cursor.h
 * Author: Morditux
 *
 * Created on January 4, 2015, 1:18 PM
 */

#ifndef CURSOR_H
#define	CURSOR_H
#include "preparedstatement.h"
#include <stdint.h>
#include "blob.h"
#include "sqliteexception.h"
#include <memory>
#include <mutex>

namespace SQLPP
{

    class Cursor;
    
    class _CursorData {
        friend Cursor;
    private:
        PreparedStatement * stmt;
        bool needReset = false;
        bool open;
        bool resultReady = false;
        std::recursive_mutex mutex;
        
    };
    
    /**
     * @brief A class to iterate over the results of an SQL query.
     * 
     * The Cursor provides methods to navigate through rows and retrieve column values.
     */
    class Cursor
    {
        friend PreparedStatement;
    private:
        Cursor(PreparedStatement * stmt);
    public:
        Cursor(const Cursor& orig);
        virtual ~Cursor();

        /**
         * @brief Close the cursor
         */
        void close();

        /**
         * @brief Check if the cursor is open
         * @return true if open, false otherwise
         */
        bool isOpen() const;

        /**
         * @brief Position the cursor on the next record if any
         * @note Must be called before retrieving values
         * @return bool true if a new record is available, false otherwise
         * @throw SQLiteException on error
         */
        bool next();

        /**
         * @brief Get column value as integer by name
         * @param columnName Name of the column
         * @return int32_t value
         */
        int32_t getAsInt(const std::string &columnName);
        /**
         * @brief Get column value as integer by index
         * @param column Index of the column (0-based)
         * @return int32_t value
         */
        int32_t getAsInt(int column);

        /**
         * @brief Get column value as 64-bit integer by name
         * @param columnName Name of the column
         * @return int64_t value
         */
        int64_t getAsLong(const std::string &columnName);
        /**
         * @brief Get column value as 64-bit integer by index
         * @param column Index of the column (0-based)
         * @return int64_t value
         */
        int64_t getAsLong(int column);
        
        /**
         * @brief Get column value as float by name
         * @param columnName Name of the column
         * @return float value
         */
        float getAsFloat(const std::string &columnName);
        /**
         * @brief Get column value as float by index
         * @param column Index of the column (0-based)
         * @return float value
         */
        float getAsFloat(int column);
        
        /**
         * @brief Get column value as double by name
         * @param columnName Name of the column
         * @return double value
         */
        double getAsDouble(const std::string &columnName);
        /**
         * @brief Get column value as double by index
         * @param column Index of the column (0-based)
         * @return double value
         */
        double getAsDouble(int column);

        /**
         * @brief Get column value as string by name
         * @param columnName Name of the column
         * @return std::string value
         */
        std::string getAsString(const std::string &columnName);
        /**
         * @brief Get column value as string by index
         * @param column Index of the column (0-based)
         * @return std::string value
         */
        std::string getAsString(int column);

        /**
         * @brief Get column value as Blob by name
         * @param columnName Name of the column
         * @return Blob value
         */
        Blob getAsBlob(const std::string &columnName);
        /**
         * @brief Get column value as Blob by index
         * @param column Index of the column (0-based)
         * @return Blob value
         */
        Blob getAsBlob(int column);

        /**
         * @brief Get the last error message from SQLite
         * @return std::string Error message
         */
        std::string errorMsg();
    private:
        void check();
        std::shared_ptr<_CursorData> d;
    };
}
#endif	/* CURSOR_H */

