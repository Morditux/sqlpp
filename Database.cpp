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
 * File:   Database.cpp
 * Author: Morditux
 * 
 * Created on January 3, 2015, 9:04 PM
 */

#include "Database.hpp"
#include "PreparedStatement.h"
#include "sqlite/sqlite3.h"
#include "SQLiteException.h"


namespace SQLPP
{
    using locker = std::lock_guard<std::recursive_mutex>;
    
    Database::Database() : d(new _DatabaseData)
    {
    }

    Database::~Database()
    {
        sqlite3_close_v2(d->db);
    }

    void Database::open(const std::string& dbName)
    {
        locker l(d->mutex);
        int result = sqlite3_open_v2(dbName.c_str(), &d->db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
        if (result != SQLITE_OK) {
            throw SQLiteException(result, errorMsg());
        }
    }

    PreparedStatement Database::prepareStatement(const std::string& sql)
    {
        locker l(d->mutex);
        PreparedStatement stmt(this);
        stmt.prepare(sql);
        return stmt;
    }

    std::string Database::errorMsg()
    {
        locker l(d->mutex);
        std::string msg(sqlite3_errmsg(d->db));
        return msg;
    }

    void Database::close()
    {
        locker l(d->mutex);
        sqlite3_close_v2(d->db);
    }

    void Database::exec(std::string sql)
    {
        locker l(d->mutex);
        if (!d->db) {
            throw SQLiteException(-1, "Database is closed / no database");
        }
        int result = sqlite3_exec(d->db, sql.c_str(), nullptr, nullptr, nullptr);
        if (result != SQLITE_OK) {
            throw SQLiteException(result, errorMsg());
        }
    }

    void Database::begin()
    {
        locker l(d->mutex);
        exec("begin");
    }

    void Database::commit()
    {
        locker l(d->mutex);
        exec("commit");
    }

    void Database::rollback()
    {
        locker l(d->mutex);
        exec("rollback");
    }
}

