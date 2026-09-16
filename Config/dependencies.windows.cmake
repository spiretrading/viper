set(mariadb_path "${PROJECT_BINARY_DIR}/Dependencies/mariadb-connector-c-3.4.9")
set(MYSQL_INCLUDE_PATH "${mariadb_path}/include")
set(MYSQL_LIBRARY_DEBUG_PATH
  "${mariadb_path}/libmariadb/Debug/mariadbclient.lib")
set(MYSQL_LIBRARY_OPTIMIZED_PATH
  "${mariadb_path}/libmariadb/Release/mariadbclient.lib")
set(SQLITE_INCLUDE_PATH
  "${PROJECT_BINARY_DIR}/Dependencies/sqlite-amalgamation-3510200")
set(SQLITE_LIBRARY_DEBUG_PATH
  "${PROJECT_BINARY_DIR}/Dependencies/sqlite-amalgamation-3510200/sqlite3d.lib")
set(SQLITE_LIBRARY_OPTIMIZED_PATH
  "${PROJECT_BINARY_DIR}/Dependencies/sqlite-amalgamation-3510200/sqlite3.lib")
