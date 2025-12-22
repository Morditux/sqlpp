# sqlpp

`sqlpp` is a lightweight, easy-to-use C++11 wrapper for the SQLite database engine. It provides a clean, object-oriented interface designed to simplify database management, query execution, and result handling while maintaining high performance.

## Architecture & Design

`sqlpp` is designed with a focus on simplicity and safety:
- **Reference Counting:** Uses `std::shared_ptr` and internal data structures (`_DatabaseData`, `_BlobData`, etc.) to manage SQLite resources automatically. Objects like `Database`, `Cursor`, and `Blob` can be safely copied; they point to the same underlying resource and ensure it is freed only when no longer needed.
- **Thread Safety:** All core classes use `std::recursive_mutex` to ensure that operations on the same database connection or prepared statement are thread-safe.
- **Exception Safety:** Errors from SQLite are translated into `SQLPP::SQLiteException`, making error handling integrated with standard C++ practices.

## Class Reference

### `SQLPP::Database`
The entry point of the library.
- `open(name)`: Connects to or creates a database file.
- `exec(sql)`: Executes raw SQL commands (ideal for DDL like `CREATE TABLE`).
- `prepareStatement(sql)`: Creates a `PreparedStatement` for parameterized queries.
- `begin()`, `commit()`, `rollback()`: Direct transaction management.

### `SQLPP::PreparedStatement`
Encapsulates a compiled SQL query.
- `prepare(sql)`: Compiles the SQL string.
- `bind(...)`: Methods to safely bind values to parameters (supports `:name` and index-based binding).
- `execute()`: Runs the query and returns a `Cursor`.
- `executeUpdate()`: Runs commands that don't return data (INSERT, UPDATE, DELETE).

### `SQLPP::Cursor`
Handles the result set of a query.
- `next()`: Advances to the next row (must be called before reading data).
- `getAsInt()`, `getAsString()`, `getAsBlob()`, etc.: Retrieve column data by name or index.

### `SQLPP::Blob`
Manages binary large objects.
- Handles memory allocation and deallocation for binary data.
- Integrated with `Cursor` for easy retrieval from the database.

### `SQLPP::SQLiteException`
Derived from `std::exception`.
- Provides the SQLite error code and a descriptive message.

## Building the Project

### Prerequisites
- C++11 compatible compiler (g++, clang++).
- SQLite3 development headers.

### Steps
1. Navigate to the project directory.
2. Run `make`:
   ```bash
   make
   ```
This produces `sqlitepp.so`, which you can link against your application.

## Advanced Usage Example

```cpp
#include <iostream>
#include "Database.hpp"
#include "PreparedStatement.h"
#include "Cursor.h"
#include "Blob.h"

int main() {
    try {
        SQLPP::Database db;
        db.open("prod.db");

        // Transaction Handling
        db.begin();
        db.exec("CREATE TABLE IF NOT EXISTS data (id INTEGER, val BLOB)");
        
        char buffer[] = {0x01, 0x02, 0x03, 0x04};
        SQLPP::Blob myBlob(4, buffer);

        SQLPP::PreparedStatement stmt(&db);
        stmt.prepare("INSERT INTO data VALUES (:id, :val)");
        stmt.setInt(":id", 101);
        stmt.setBlob(":val", myBlob);
        stmt.executeUpdate();
        
        db.commit();

        // Reading back
        SQLPP::PreparedStatement select(&db);
        select.prepare("SELECT * FROM data WHERE id = ?");
        select.setInt(1, 101);
        
        SQLPP::Cursor cursor = select.execute();
        if (cursor.next()) {
            SQLPP::Blob retrieved = cursor.getAsBlob("val");
            std::cout << "Retrieved blob of size: " << retrieved.size() << std::endl;
        }

    } catch (const SQLPP::SQLiteException& e) {
        std::cerr << "Error [" << e.errorCode() << "]: " << e.what() << std::endl;
    }
    return 0;
}
```

## License
Distributed under the MIT License. See `LICENSE` for more information.
