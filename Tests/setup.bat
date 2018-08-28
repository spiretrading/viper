SETLOCAL

pushd %~dp0
mkdir ThirdParty
pushd ThirdParty
if exist sqlite goto end_sqlite_setup
  wget --no-check-certificate https://www.sqlite.org/2018/sqlite-amalgamation-3230100.zip
  if not exist sqlite-amalgamation-3230100.zip goto end_sqlite_setup
    unzip sqlite-amalgamation-3230100
    mv sqlite-amalgamation-3230100 sqlite
    pushd sqlite
    cl /c /O2 sqlite3.c
    lib sqlite3.obj
    popd
    rm sqlite-amalgamation-3230100.zip
:end_sqlite_setup

if exist Catch2 goto end_catch_setup
  git clone --branch v2.2.1 https://github.com/catchorg/Catch2.git Catch2
:end_catch_setup

popd
popd

ENDLOCAL
