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
#include "PreparedStatement.h"
#include <stdint.h>
#include "Blob.h"
#include "SQLiteException.h"
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
    
    class Cursor
    {
        friend PreparedStatement;
    private:
        Cursor(PreparedStatement * stmt);
    public:
        Cursor(const Cursor& orig);
        virtual ~Cursor();

        void close();

        bool isOpen() const;

        /**
         * Position the cursor on the next record if any, must be called before retrieving values
         * @return true
         * @throw SQLiteException on error
         */
        bool next();

        int32_t getAsInt(const std::string &columnName);
        int32_t getAsInt(int column);

        int64_t getAsLong(const std::string &columnName);
        int64_t getAsLong(int column);
        
        float getAsFloat(const std::string &columnName);
        float getAsFloat(int column);
        
        double getAsDouble(const std::string &columnName);
        double getAsDouble(int column);

        std::string getAsString(const std::string &columnName);
        std::string getAsString(int column);

        Blob getAsBlob(const std::string &columnName);
        Blob getAsBlob(int column);

        std::string errorMsg();
    private:
        void check();
        std::shared_ptr<_CursorData> d;
    };
}
#endif	/* CURSOR_H */

