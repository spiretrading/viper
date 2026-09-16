set(mariadb_path "${PROJECT_BINARY_DIR}/Dependencies/mariadb-connector-c-3.4.9")
set(MYSQL_INCLUDE_PATH "${mariadb_path}/include")
set(MYSQL_LIBRARY_DEBUG_PATH "${mariadb_path}/libmariadb/libmariadbclient.a")
set(MYSQL_LIBRARY_OPTIMIZED_PATH
  "${mariadb_path}/libmariadb/libmariadbclient.a")
set(openssl_path "${PROJECT_BINARY_DIR}/Dependencies/openssl-3.6.0")
set(OPEN_SSL_INCLUDE_PATH "${openssl_path}/include")
unset(OPEN_SSL_BASE_LIBRARY_DEBUG_PATH)
unset(OPEN_SSL_LIBRARY_DEBUG_PATH)
find_library(OPEN_SSL_BASE_LIBRARY_DEBUG_PATH NAMES libcrypto.a
  PATHS "${openssl_path}/lib" "${openssl_path}/lib64"
  NO_DEFAULT_PATH NO_CACHE REQUIRED)
find_library(OPEN_SSL_LIBRARY_DEBUG_PATH NAMES libssl.a
  PATHS "${openssl_path}/lib" "${openssl_path}/lib64"
  NO_DEFAULT_PATH NO_CACHE REQUIRED)
set(OPEN_SSL_BASE_LIBRARY_OPTIMIZED_PATH "${OPEN_SSL_BASE_LIBRARY_DEBUG_PATH}")
set(OPEN_SSL_LIBRARY_OPTIMIZED_PATH "${OPEN_SSL_LIBRARY_DEBUG_PATH}")
set(SQLITE_INCLUDE_PATH
  "${PROJECT_BINARY_DIR}/Dependencies/sqlite-amalgamation-3510200")
set(SQLITE_LIBRARY_DEBUG_PATH
  "${PROJECT_BINARY_DIR}/Dependencies/sqlite-amalgamation-3510200/sqlite3.lib")
set(SQLITE_LIBRARY_OPTIMIZED_PATH
  "${PROJECT_BINARY_DIR}/Dependencies/sqlite-amalgamation-3510200/sqlite3.lib")
