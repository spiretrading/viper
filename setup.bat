@ECHO OFF
SETLOCAL EnableDelayedExpansion
SET "ROOT=%cd%"
CALL :CheckCache "viper"
IF ERRORLEVEL 1 EXIT /B 0
CALL :SetupVSEnvironment
CALL :AddDependency "doctest-2.4.12" ^
  "https://github.com/doctest/doctest/archive/refs/tags/v2.4.12.zip" ^
  "7a7afb5f70d0b749d49ddfcb8a454299a8fcd53e9db9c131abe99b456e88a1fe"
CALL :AddDependency "sqlite-amalgamation-3510200" ^
  "https://www.sqlite.org/2026/sqlite-amalgamation-3510200.zip" ^
  "6e2a845a493026bdbad0618b2b5a0cf48584faab47384480ed9f592d912f23ec" ^
  ":BuildSQLite"
SET "MARIADB_URL=https://github.com/mariadb-corporation/mariadb-connector-c"
CALL :AddDependency "mariadb-connector-c-3.4.9" ^
  "!MARIADB_URL!/archive/refs/tags/v3.4.9.zip" ^
  "2342f6e58907f7431b5ccafb8b8e744b6b0e64174d72395d2330576b8a535fb6" ^
  ":BuildMariaDB"
CALL :InstallDependencies || EXIT /B 1
CALL :Commit
EXIT /B !ERRORLEVEL!
ENDLOCAL

:BuildSQLite
cl /c /Zi /MDd /DSQLITE_USE_URI=1 sqlite3.c || EXIT /B 1
lib sqlite3.obj || EXIT /B 1
COPY /Y sqlite3.lib sqlite3d.lib || EXIT /B 1
DEL sqlite3.obj || EXIT /B 1
cl /c /O2 /MD /DSQLITE_USE_URI=1 sqlite3.c || EXIT /B 1
lib sqlite3.obj || EXIT /B 1
EXIT /B 0

:BuildMariaDB
SETLOCAL
SET "CMAKE_GENERATOR="
SET "CMAKE_GENERATOR_PLATFORM="
SET "CMAKE_GENERATOR_TOOLSET="
SET "CMAKE_GENERATOR_INSTANCE="
cmake --fresh -A x64 -DCMAKE_INSTALL_PREFIX=./mariadb ^
  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW ^
  "-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" ^
  -DCLIENT_PLUGIN_CACHING_SHA2_PASSWORD=STATIC . || (ENDLOCAL & EXIT /B 1)
FOR %%C IN (Debug Release) DO (
  cmake --build . --target mariadbclient --config %%C || (ENDLOCAL & EXIT /B 1)
)
ENDLOCAL
EXIT /B 0

:CheckCache
SET "CACHE_NAME=%~1"
SET "SETUP_HASH="
FOR /F "skip=1" %%H IN ('certutil -hashfile "%~dp0setup.bat" SHA256') DO (
  IF NOT DEFINED SETUP_HASH SET "SETUP_HASH=%%H"
)
IF EXIST "cache_files\!CACHE_NAME!.txt" (
  SET /P CACHED_HASH=<"cache_files\!CACHE_NAME!.txt"
  IF "!SETUP_HASH!"=="!CACHED_HASH!" EXIT /B 1
)
EXIT /B 0

:Commit
IF NOT EXIST cache_files (
  MD cache_files || EXIT /B 1
)
>"cache_files\!CACHE_NAME!.txt" ECHO !SETUP_HASH!
EXIT /B 0

:SetupVSEnvironment
SET "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
FOR /F "usebackq delims=" %%i IN (` ^
    "!VSWHERE!" -prerelease -latest -products * ^
      -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
      -property installationPath`) DO (
  IF EXIST "%%i\Common7\Tools\vsdevcmd.bat" (
    CALL "%%i\Common7\Tools\vsdevcmd.bat" -arch=x64 -host_arch=x64
  )
)
EXIT /B 0

:AddDependency
IF NOT DEFINED NEXT_DEPENDENCY_INDEX SET "NEXT_DEPENDENCY_INDEX=0"
SET "DEPENDENCIES[%NEXT_DEPENDENCY_INDEX%].NAME=%~1"
SET "DEPENDENCIES[%NEXT_DEPENDENCY_INDEX%].URL=%~2"
SET "DEPENDENCIES[%NEXT_DEPENDENCY_INDEX%].HASH=%~3"
SET "DEPENDENCIES[%NEXT_DEPENDENCY_INDEX%].BUILD=%~4"
SET /A NEXT_DEPENDENCY_INDEX+=1
EXIT /B 0

:InstallDependencies
SET "I=0"
:InstallDependenciesLoop
IF NOT DEFINED DEPENDENCIES[%I%].NAME EXIT /B 0
CALL :DownloadAndExtract "!DEPENDENCIES[%I%].NAME!" "!DEPENDENCIES[%I%].URL!" ^
  "!DEPENDENCIES[%I%].HASH!" "!DEPENDENCIES[%I%].BUILD!" || EXIT /B 1
SET /A I+=1
GOTO InstallDependenciesLoop

:DownloadAndExtract
SET "FOLDER=%~1"
SET "URL=%~2"
SET "EXPECTED_HASH=%~3"
SET "BUILD_LABEL=%~4"
SET "ACTUAL_HASH="
FOR /F "tokens=* delims=/" %%A IN ("!URL!") DO (
  SET "ARCHIVE=%%~nxA"
)
IF EXIST "!FOLDER!" (
  IF EXIST "!FOLDER!\.viper_build_complete" EXIT /B 0
  IF EXIST "!FOLDER!\.viper_extract_complete" GOTO BuildDependency
)
IF NOT EXIST "!ARCHIVE!" (
  curl -fsL -o "!ARCHIVE!" "!URL!" || EXIT /B 1
)
FOR /F "skip=1 tokens=*" %%H IN ('certutil -hashfile "!ARCHIVE!" SHA256') DO (
  IF NOT DEFINED ACTUAL_HASH SET "ACTUAL_HASH=%%H"
)
SET "ACTUAL_HASH=!ACTUAL_HASH: =!"
IF /I NOT "!ACTUAL_HASH!"=="!EXPECTED_HASH!" (
  ECHO Error: SHA256 mismatch for !ARCHIVE!.
  ECHO   Expected: !EXPECTED_HASH!
  ECHO   Actual:   !ACTUAL_HASH!
  DEL /F /Q "!ARCHIVE!"
  SET "ACTUAL_HASH="
  EXIT /B 1
)
SET "ACTUAL_HASH="
IF NOT EXIST "!FOLDER!" (
  MD "!FOLDER!" || EXIT /B 1
)
tar -xf "!ARCHIVE!" --strip-components=1 -C "!FOLDER!" || EXIT /B 1
TYPE NUL > "!FOLDER!\.viper_extract_complete" || EXIT /B 1
:BuildDependency
IF DEFINED BUILD_LABEL (
  PUSHD "!FOLDER!" || EXIT /B 1
  CALL !BUILD_LABEL!
  SET "BUILD_RESULT=!ERRORLEVEL!"
  POPD
  IF NOT "!BUILD_RESULT!"=="0" EXIT /B !BUILD_RESULT!
  TYPE NUL > "!FOLDER!\.viper_build_complete" || EXIT /B 1
)
IF EXIST "!ARCHIVE!" DEL /F /Q "!ARCHIVE!"
EXIT /B 0
