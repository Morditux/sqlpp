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
 * File:   Cursor.cpp
 * Author: Morditux
 * 
 * Created on January 4, 2015, 1:18 PM
 */

#include "cursor.h"
namespace SQLPP
{

    using locker = std::lock_guard<std::recursive_mutex>;

    Cursor::Cursor(PreparedStatement * stmt) : d(new _CursorData)
    {
        locker l(d->mutex);
        d->stmt = stmt;
        /* A cursor is opened at creation*/
        d->open = true;
        stmt->d->excecuted = true;
        stmt->d->cursorClosed = false;
    }

    Cursor::Cursor(const Cursor& orig)
    {
        locker l(d->mutex);
        d = orig.d;
    }

    Cursor::~Cursor()
    {
        locker l(d->mutex);
        close();
    }

    void Cursor::close()
    {
        locker l(d->mutex);
        d->open = false;
        d->stmt->d->cursorClosed = true;
    }

    bool Cursor::isOpen() const
    {
        locker l(d->mutex);
        return d->open;
    }

    bool Cursor::next()
    {
        locker l(d->mutex);
        if (!d->open) {
            return false;
        }
        if (d->needReset) {
            return false;
        }

        int result = d->stmt->next();

        if (result == SQLITE_ROW) {
            // A new row is ready
            d->resultReady = true;
            return true;
        }

        if (result == SQLITE_DONE) {
            // No more rows
            d->resultReady = false;
            d->needReset = true;
            return false;
        }

        // oops
        d->needReset = true;
        throw SQLiteException(result, errorMsg());

    }

    void Cursor::check()
    {
        locker l(d->mutex);
        if (!d->resultReady) {
            throw SQLiteException(-1, "Cursor operation error - statement is not ready, no row to proceed");
        }
    }

    int32_t Cursor::getAsInt(const std::string &columnName)
    {

        return getAsInt(d->stmt->columnNumber(columnName));
    }

    int32_t Cursor::getAsInt(int column)
    {
        locker l(d->mutex);
        check();
        int32_t value = sqlite3_column_int(d->stmt->d->stmt, column);
        return value;
    }

    int64_t Cursor::getAsLong(const std::string &columnName)
    {

        return getAsLong(d->stmt->columnNumber(columnName));
    }

    int64_t Cursor::getAsLong(int column)
    {
        locker l(d->mutex);
        check();
        int64_t value = sqlite3_column_int64(d->stmt->d->stmt, column);
        return value;
    }

    float Cursor::getAsFloat(const std::string &columnName)
    {
        return getAsFloat(d->stmt->columnNumber(columnName));
    }

    float Cursor::getAsFloat(int column)
    {
        locker l(d->mutex);
        check();
        double value = sqlite3_column_double(d->stmt->d->stmt, column);
        return static_cast<float> (value);
    }

    double Cursor::getAsDouble(const std::string &columnName)
    {
        return getAsDouble(d->stmt->columnNumber(columnName));
    }

    double Cursor::getAsDouble(int column)
    {
        locker l(d->mutex);
        check();
        double value = sqlite3_column_double(d->stmt->d->stmt, column);
        return value;
    }

    std::string Cursor::getAsString(const std::string &columnName)
    {
        return getAsString(d->stmt->columnNumber(columnName));
    }

    std::string Cursor::getAsString(int column)
    {
        locker l(d->mutex);
        check();
        const char * data = reinterpret_cast<const char *> (sqlite3_column_text(d->stmt->d->stmt, column));
        std::string value(data);
        return value;
    }

    Blob Cursor::getAsBlob(const std::string &columnName)
    {
        return getAsBlob(d->stmt->columnNumber(columnName));
    }

    Blob Cursor::getAsBlob(int column)
    {
        locker l(d->mutex);
        check();
        const char * data = static_cast<const char *> (sqlite3_column_blob(d->stmt->d->stmt, column));
        int size = sqlite3_column_bytes(d->stmt->d->stmt, column);
        Blob blob(size, data);
        return blob;
    }

    std::string Cursor::errorMsg()
    {
        locker l(d->mutex);
        return d->stmt->errorMsg();
    }

}
