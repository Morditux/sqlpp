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
 * 
 * File:   PreparedStatement.cpp
 * Author: Morditux
 * 
 * Created on January 4, 2015, 9:06 AM
 */

#include "PreparedStatement.h"
#include "SQLiteException.h"
#include "Cursor.h"


namespace SQLPP
{
    using locker = std::lock_guard<std::recursive_mutex>;

    PreparedStatement::PreparedStatement(Database *db) : d(new _PreparedStatementData)
    {
        d->db = db;
    }

    PreparedStatement::~PreparedStatement()
    {
        locker l(d->mutex);
        finalize();
    }

    void PreparedStatement::setSignalDeletion(bool value)
    {
        locker l(d->mutex);
        d->signalDeletion = value;
    }

    void PreparedStatement::prepare(const std::string& sql)
    {
        locker l(d->mutex);
        if (d->prepared) {
            throw SQLiteException(-1, "Statement is already prepared");
        }

        if (d->db == nullptr) {
            throw SQLiteException(-1, "PreparedStatement::prepare : Database pointer is null");
        }
        int result = sqlite3_prepare_v2(d->db->getSqltite3db(), sql.c_str(), sql.size() + 1, &d->stmt, nullptr);
        if (result != SQLITE_OK) {
            throw SQLiteException(result, errorMsg());
        }
        // The statement is ready
        d->prepared = true;
        // Count columns
        int count = sqlite3_column_count(d->stmt);

        // Add column index and name to d->_columnsNames
        d->columnsNames->clear();
        for (int i = 0; i < count; i++) {
            std::string name(sqlite3_column_name(d->stmt, i));
            d->columnsNames->emplace(name, i);
        }

    }

    void PreparedStatement::finalize()
    {
        locker l(d->mutex);
        if (!d->prepared) {
            // Don't try to finalize unprepared statement
            return;
        }
        if ((d->db != nullptr) && (d->stmt != nullptr)) {
            if (sqlite3_finalize(d->stmt) != SQLITE_OK) {
                throw SQLiteException(sqlite3_errcode(d->db->getSqltite3db()), errorMsg());
            }
            d->prepared = false;
            d->stmt == nullptr;
            d->excecuted = false;
        }
    }

    void PreparedStatement::close()
    {
        locker l(d->mutex);
        finalize();
    }

    int PreparedStatement::next()
    {
        locker l(d->mutex);
        if ((d->db != nullptr) && (d->stmt != nullptr)) {
            int result = sqlite3_step(d->stmt);
            return result;
        }
        return SQLITE_ERROR;
    }

    std::string PreparedStatement::errorMsg()
    {
        locker l(d->mutex);
        return d->db->errorMsg();
    }

    bool PreparedStatement::isValid() const
    {
        locker l(d->mutex);
        return d->prepared;
    }

    int PreparedStatement::columnNumber(const std::string &name) const
    {
        locker l(d->mutex);
        auto it = d->columnsNames->find(name);
        if (it == d->columnsNames->end()) {
            throw SQLiteException(-1, "PreparedStatement::columnName - Invalid column number");
        }
        return it->second;
    }

    void PreparedStatement::executeUpdate()
    {
        locker l(d->mutex);
        int result = next();
        if (result != SQLITE_OK && result != SQLITE_DONE) {
            throw SQLiteException(result, errorMsg());
        }
        sqlite3_reset(d->stmt);
    }

    Cursor PreparedStatement::execute()
    {
        locker l(d->mutex);
        if (!d->cursorClosed) {
            throw SQLiteException(-1, "Current Cursor must be closed before executing prepared statement");
        }

        if (d->excecuted) {
            sqlite3_reset(d->stmt);
        }

        Cursor c(this);
        return c;
    }

    void PreparedStatement::setInt(const std::string &paramName, int32_t value)
    {

        setInt(sqlite3_bind_parameter_index(d->stmt, paramName.c_str()), value);
    }

    void PreparedStatement::setInt(int column, int32_t value)
    {
        locker l(d->mutex);
        if (!d->prepared) {
            return;
        }
        sqlite3_bind_int(d->stmt, column, value);
    }

    void PreparedStatement::setLong(const std::string &paramName, int64_t value)
    {

        setLong(sqlite3_bind_parameter_index(d->stmt, paramName.c_str()), value);
    }

    void PreparedStatement::setLong(int column, int64_t value)
    {
        locker l(d->mutex);
        if (!d->prepared) {
            return;
        }
        sqlite3_bind_int64(d->stmt, column, value);
    }

    void PreparedStatement::setFloat(const std::string &paramName, float value)
    {

        setFloat(sqlite3_bind_parameter_index(d->stmt, paramName.c_str()), value);
    }

    void PreparedStatement::setFloat(int column, float value)
    {
        locker l(d->mutex);
        if (!d->prepared) {
            return;
        }
        sqlite3_bind_double(d->stmt, column, static_cast<float> (value));
    }

    void PreparedStatement::setDouble(const std::string &paramName, double value)
    {

        setDouble(sqlite3_bind_parameter_index(d->stmt, paramName.c_str()), value);
    }

    void PreparedStatement::setDouble(int column, double value)
    {
        locker l(d->mutex);
        if (!d->prepared) {
            return;
        }
        sqlite3_bind_double(d->stmt, column, value);
    }

    void PreparedStatement::setString(const std::string &paramName, const std::string &value)
    {

        setString(sqlite3_bind_parameter_index(d->stmt, paramName.c_str()), value);
    }

    void PreparedStatement::setString(int column, const std::string &value)
    {
        locker l(d->mutex);
        if (!d->prepared) {
            return;
        }
        sqlite3_bind_text(d->stmt, column, value.c_str(), value.size() + 1, SQLITE_STATIC);
    }

    void PreparedStatement::setBlob(const std::string &paramName, const Blob &value)
    {

        setBlob(sqlite3_bind_parameter_index(d->stmt, paramName.c_str()), value);
    }

    void PreparedStatement::setBlob(int column, const Blob &value)
    {
        locker l(d->mutex);
        if (!d->prepared) {
            return;
        }
        sqlite3_bind_blob(d->stmt, column, value.data(), value.size(), SQLITE_STATIC);
    }
}
