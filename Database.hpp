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
 * File:   Database.hpp
 * Author: Morditux
 *
 * Created on January 3, 2015, 9:04 PM
 */

#ifndef DATABASE_HPP
#define	DATABASE_HPP
#include "sqlite/sqlite3.h"
#include <string>
#include <memory>
#include <mutex>

namespace SQLPP
{
    class PreparedStatement;
    class Database;

    class _DatabaseData
    {
        friend Database;
    private:
        sqlite3 * db = 0;
        bool inTransaction = false;
        std::recursive_mutex mutex;
    };

    /**
     * @brief Main class for interacting with an SQLite database.
     * 
     * This class provides methods to open/close databases, execute raw SQL,
     * and manage transactions.
     */
    class Database
    {
    public:
        friend PreparedStatement;
        /**
         * @brief Construct a new Database object
         */
        Database();

        /**
         * @brief Deleted copy constructor
         */
        Database(const Database& orig) = delete;
        /**
         * @brief Destroy the Database object
         */
        virtual ~Database();

        /**
         * @brief Open database. Create a new database file if it does not exist.
         * @param dbName The database file name
         * @throw SQLiteException on error
         */
        void open(const std::string & dbName);
        /**
         * @brief Close the database connection
         */
        void close();
        /**
         * @brief Get the last error message from SQLite
         * @return std::string Error message
         */
        std::string errorMsg();

        /**
         * @brief Prepare an SQL statement
         * @param sql The SQL query string
         * @return PreparedStatement The prepared statement object
         */
        PreparedStatement prepareStatement(const std::string &sql);

        /**
         * @brief Execute a raw SQL statement
         * @param sql The SQL query string
         */
        void exec(std::string sql);
        /**
         * @brief Begin a transaction
         */
        void begin();
        /**
         * @brief Commit the current transaction
         */
        void commit();
        /**
         * @brief Rollback the current transaction
         */
        void rollback();


    private:

        sqlite3 * getSqltite3db() const
        {
            return d->db;
        }

        std::shared_ptr<_DatabaseData> d;
    };

}
#endif	/* DATABASE_HPP */

