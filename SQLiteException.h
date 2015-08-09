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
 * File:   SQLiteException.h
 * Author: Morditux
 *
 * Created on January 6, 2015, 9:21 PM
 */

#ifndef SQLITEEXCEPTION_H
#define	SQLITEEXCEPTION_H
#include <memory>
#include <cstring>
#include <exception>
#include <string>

namespace SQLPP
{
    /**
     * We can't embed std::string in Exception class but we need reference counted string. 
     * This class is a simple helper for reference counter string embedded in exception...
     */
    class SQLiteException;
    
    class _SQLiteExceptionData
    {
        friend SQLiteException;
    public:
        _SQLiteExceptionData(const std::string &msg) 
        {
            std::string s("SQLiteException : ");
            s.append(msg);
            int size = s.size();

            string = static_cast<char *>(::malloc(size+1));
            memcpy(string,s.c_str(),size);
            string[size]=0;
        }
        
        ~_SQLiteExceptionData() 
        {
            ::free(string);
        }
        
      
    private:
        char * string;
        int errCode;
    };
    using SQLiteExceptionStringPtr = std::shared_ptr<_SQLiteExceptionData>;
            
    
    class SQLiteException : public std::exception
    {
    public:
        SQLiteException(int errCode, const std::string &msg);
        SQLiteException(const SQLiteException& orig);
        virtual ~SQLiteException();
        const char * what() const throw() override;
        int errorCode() const;
    private:
        SQLiteExceptionStringPtr d;
        
    };
}
#endif	/* SQLITEEXCEPTION_H */

