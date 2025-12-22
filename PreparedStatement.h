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
 * File:   PreparedStatement.h
 * Author: Morditux
 *
 * Created on January 4, 2015, 9:06 AM
 */

#ifndef PREPAREDSTATEMENT_H
#define	PREPAREDSTATEMENT_H
#include "Database.hpp"
#include <string>
#include "sqlite/sqlite3.h"
#include "Blob.h"
#include <unordered_map>
#include <memory>
#include <mutex>

namespace SQLPP
{
    class Cursor;
    class PreparedStatement;

    class _PreparedStatementData
    {
        friend PreparedStatement;
        friend Cursor;
    public:

        _PreparedStatementData()
        {
            columnsNames = new std::unordered_map<std::string, int>();
        }

        ~_PreparedStatementData()
        {
            delete columnsNames;
        }
    private:
        sqlite3_stmt * stmt = 0;
        bool signalDeletion = true;
        bool prepared = false;
        bool excecuted = false;
        bool cursorClosed = true;
        std::recursive_mutex mutex;
        std::unordered_map<std::string, int> *columnsNames;
        Database * db;
    };

    /**
     * @brief A class representing a prepared SQL statement.
     * 
     * PreparedStatement allows for efficient execution of SQL queries with parameter binding,
     * protecting against SQL injection and improving performance for repeated queries.
     */
    class PreparedStatement
    {
        friend Database;
        friend Cursor;
    public:
        /**
         * @brief Construct a new Prepared Statement object
         * @param db Pointer to the Database object
         */
        PreparedStatement(Database * db);
        /**
         * @brief Destroy the Prepared Statement object
         */
        virtual ~PreparedStatement();

        /**
         * @brief Prepare a new SQL statement
         * @param sql The SQL query string
         * @throw SQLiteException on error
         */
        void prepare(const std::string &sql);
        /**
         * @brief Execute an update statement (INSERT, UPDATE, DELETE)
         */
        void executeUpdate();
        /**
         * @brief Execute a query statement (SELECT)
         * @return Cursor object to iterate through results
         */
        Cursor execute();
        /**
         * @brief Get the last error message from SQLite
         * @return std::string Error message
         */
        std::string errorMsg();
        /**
         * @brief Check if the statement is valid and prepared
         * @return true if valid, false otherwise
         */
        bool isValid() const;

        /**
         * @brief Close the statement and free resources
         */
        void close();
        /**
         * @brief Get the index of a column by its name
         * @param name Name of the column
         * @return int Index of the column
         */
        int columnNumber(const std::string &name) const;
        /**
         * @brief Bind an integer value to a named parameter
         * @param paramName Name of the parameter (e.g., ":id")
         * @param value Integer value to bind
         */
        void setInt(const std::string &paramName, int32_t value);
        /**
         * @brief Bind an integer value to a parameter by index
         * @param column Index of the parameter (1-based)
         * @param value Integer value to bind
         */
        void setInt(int column, int32_t value);
        /**
         * @brief Bind a long value to a named parameter
         * @param paramName Name of the parameter
         * @param value Long value to bind
         */
        void setLong(const std::string &paramName, int64_t value);
        /**
         * @brief Bind a long value to a parameter by index
         * @param column Index of the parameter (1-based)
         * @param value Long value to bind
         */
        void setLong(int column, int64_t value);
        /**
         * @brief Bind a float value to a named parameter
         * @param paramName Name of the parameter
         * @param value Float value to bind
         */
        void setFloat(const std::string &paramName, float value);
        /**
         * @brief Bind a float value to a parameter by index
         * @param column Index of the parameter (1-based)
         * @param value Float value to bind
         */
        void setFloat(int column, float value);
        /**
         * @brief Bind a double value to a named parameter
         * @param paramName Name of the parameter
         * @param value Double value to bind
         */
        void setDouble(const std::string &paramName, double value);
        /**
         * @brief Bind a double value to a parameter by index
         * @param column Index of the parameter (1-based)
         * @param value Double value to bind
         */
        void setDouble(int column, double value);
        /**
         * @brief Bind a string value to a named parameter
         * @param paramName Name of the parameter
         * @param value String value to bind
         */
        void setString(const std::string &paramName, const std::string &value);
        /**
         * @brief Bind a string value to a parameter by index
         * @param column Index of the parameter (1-based)
         * @param value String value to bind
         */
        void setString(int column, const std::string &value);
        /**
         * @brief Bind a Blob value to a named parameter
         * @param paramName Name of the parameter
         * @param value Blob value to bind
         */
        void setBlob(const std::string &paramName, const Blob &value);
        /**
         * @brief Bind a Blob value to a parameter by index
         * @param column Index of the parameter (1-based)
         * @param value Blob value to bind
         */
        void setBlob(int column, const Blob &value);

    protected:
        /**
         * 
         * @param value
         */
        void setSignalDeletion(bool value);
        /**
         * 
         */
        void finalize();
        /**
         * 
         * @return 
         */
        int next();
    private:
        std::shared_ptr<_PreparedStatementData> d;
    };
}
#endif	/* PREPAREDSTATEMENT_H */

