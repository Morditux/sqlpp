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

    class PreparedStatement
    {
        friend Database;
        friend Cursor;
    public:
        /**
         * 
         * @param db
         */
        PreparedStatement(Database * db);
        /**
         * 
         */
        virtual ~PreparedStatement();

        /**
         * Create a new prepared statement
         * @param sql The SQL String
         * @throw SQLiteException on error
         */
        void prepare(const std::string &sql);
        /**
         * 
         */
        void executeUpdate();
        /**
         * 
         * @return 
         */
        Cursor execute();
        /**
         * 
         * @return 
         */
        std::string errorMsg();
        /**
         * 
         * @return 
         */
        bool isValid() const;

        /**
         * Close the statement and free memory
         */
        void close();
        /**
         * 
         * @param name
         * @return 
         */
        int columnNumber(const std::string &name) const;
        /**
         * 
         * @param paramName
         * @param value
         */
        void setInt(const std::string &paramName, int32_t value);
        /**
         * 
         * @param column
         * @param value
         */
        void setInt(int column, int32_t value);
        /**
         * 
         * @param paramName
         * @param value
         */
        void setLong(const std::string &paramName, int64_t value);
        /**
         * 
         * @param column
         * @param value
         */
        void setLong(int column, int64_t value);
        /**
         * 
         * @param paramName
         * @param value
         */
        void setFloat(const std::string &paramName, float value);
        /**
         * 
         * @param column
         * @param value
         */
        void setFloat(int column, float value);
        /**
         * 
         * @param paramName
         * @param value
         */
        void setDouble(const std::string &paramName, double value);
        /**
         * 
         * @param column
         * @param value
         */
        void setDouble(int column, double value);
        /**
         * 
         * @param paramName
         * @param value
         */
        void setString(const std::string &paramName, const std::string &value);
        /**
         * 
         * @param column
         * @param value
         */
        void setString(int column, const std::string &value);
        /**
         * 
         * @param paramName
         * @param value
         */
        void setBlob(const std::string &paramName, const Blob &value);
        /**
         * 
         * @param column
         * @param value
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

