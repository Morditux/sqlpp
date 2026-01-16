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
 * File:   example.cpp
 * Author: Morditux
 *
 * Created on January 3, 2015, 6:43 PM
 */

#include <cstdlib>
#include <iostream>
#include "database.hpp"
#include "preparedstatement.h"
#include "sqliteexception.h"
#include "cursor.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    try {
        SQLPP::Database db;
        db.open("test.db");
        db.exec("create table test (a int,b text)");
        
        
        SQLPP::PreparedStatement stmt(&db);
        stmt.prepare("insert into test values (:aval,:bval)");
        stmt.setInt(":aval",1);
        stmt.setString(":bval","toto la mouche qui tue !");
        stmt.executeUpdate();
        stmt.close();

        SQLPP::PreparedStatement selectStatement(&db);
        selectStatement.prepare("select * from test");
        SQLPP::Cursor c = selectStatement.execute();
        while (c.next()) {
            cout << "Data : " << c.getAsString("a") << "\n";
            cout << "Data : " << c.getAsString("b") << "\n";
        }
        c.close();
        selectStatement.close();

    }
    catch (std::exception &e) {
        cout << e.what();
    }

    return 0;
}

