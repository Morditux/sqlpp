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

    class Database
    {
    public:
        friend PreparedStatement;
        /**
         * 
         */
        Database();

        /**
         * 
         * @param orig
         */
        Database(const Database& orig) = delete;
        virtual ~Database();

        /**
         * Open database. Create a new database if the database does not exists
         * @param dbName The database file name
         * @throw SQLiteException on error
         */
        void open(const std::string & dbName);
        /**
         * 
         */
        void close();
        /**
         * 
         * @return 
         */
        std::string errorMsg();

        /**
         * 
         * @param sql
         * @return 
         */
        PreparedStatement prepareStatement(const std::string &sql);

        /**
         * 
         * @param sql
         */
        void exec(std::string sql);
        /**
         * 
         */
        void begin();
        /**
         * 
         */
        void commit();
        /**
         * 
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

