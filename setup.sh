#!/bin/bash
set -o errexit
set -o pipefail
DIRECTORY=""
ROOT=""
CACHE_DIRECTORY=""
DEPENDENCIES=()

sha256() {
  if command -v sha256sum >/dev/null; then
    sha256sum "$1" | cut -d" " -f1
  else
    shasum -a 256 "$1" | cut -d" " -f1
  fi
}

get_core_count() {
  nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4
}

main() {
  resolve_paths
  CACHE_DIRECTORY="$ROOT/cache_files/viper"
  mkdir -p "$CACHE_DIRECTORY" || return 1
  add_dependency "doctest-2.4.12" \
    "https://github.com/doctest/doctest/archive/refs/tags/v2.4.12.zip" \
    "7a7afb5f70d0b749d49ddfcb8a454299a8fcd53e9db9c131abe99b456e88a1fe" 1
  add_dependency "sqlite-amalgamation-3510200" \
    "https://www.sqlite.org/2026/sqlite-amalgamation-3510200.zip" \
    "6e2a845a493026bdbad0618b2b5a0cf48584faab47384480ed9f592d912f23ec" 1 \
    "build_sqlite"
  local openssl_url="https://github.com/openssl/openssl/releases/download"
  add_dependency "openssl-3.6.0-build" \
    "$openssl_url/openssl-3.6.0/openssl-3.6.0.tar.gz" \
    "b6a5f44b7eb69e3fa35dbf15524405b44837a481d43d81daddde3ff21fcbb8e9" 1 \
    "build_openssl"
  local mariadb_url="https://github.com/mariadb-corporation/mariadb-connector-c"
  add_dependency "mariadb-connector-c-3.4.9" \
    "$mariadb_url/archive/refs/tags/v3.4.9.zip" \
    "2342f6e58907f7431b5ccafb8b8e744b6b0e64174d72395d2330576b8a535fb6" 1 \
    "build_mariadb"
  install_dependencies || return 1
}

build_sqlite() {
  gcc -c -O2 -o sqlite3.lib -DSQLITE_USE_URI=1 -fPIC sqlite3.c || return 1
}

build_openssl() {
  local cores
  cores=$(get_core_count)
  export LDFLAGS=-ldl
  ./config no-shared no-tests threads -fPIC -ldl \
    --prefix="$ROOT/openssl-3.6.0" || return 1
  make -j "$cores" || return 1
  make install || return 1
  unset LDFLAGS
}

build_mariadb() {
  local cores
  cores=$(get_core_count)
  export OPENSSL_ROOT_DIR="$ROOT/openssl-3.6.0"
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=./mariadb -DWITH_UNIT_TESTS=OFF \
    -DCLIENT_PLUGIN_CACHING_SHA2_PASSWORD=STATIC . || return 1
  make -j "$cores" || return 1
  make install || return 1
  unset OPENSSL_ROOT_DIR
}

resolve_paths() {
  local source="${BASH_SOURCE[0]}"
  while [[ -h "$source" ]]; do
    local dir="$(cd -P "$(dirname "$source")" >/dev/null && pwd -P)"
    source="$(readlink "$source")"
    [[ $source != /* ]] && source="$dir/$source"
  done
  DIRECTORY="$(cd -P "$(dirname "$source")" >/dev/null && pwd -P)"
  ROOT="$(pwd -P)"
}

add_dependency() {
  local name="$1"
  local url="$2"
  local hash="$3"
  local revision="$4"
  local build="${5:-}"
  DEPENDENCIES+=("$name|$url|$hash|$revision|$build")
}

install_dependencies() {
  for dep in "${DEPENDENCIES[@]}"; do
    IFS='|' read -r name url hash revision build <<< "$dep"
    download_and_extract "$name" "$url" "$hash" "$revision" "$build" ||
      return 1
  done
}

download_and_extract() {
  local folder="$1"
  local build_marker="$CACHE_DIRECTORY/$folder.build_complete"
  local url="$2"
  local expected_hash="$3"
  local build_hash="$expected_hash posix-$4"
  local build_func="$5"
  local archive="${url##*/}"
  if [[ -d "$folder" && -f "$build_marker" ]] &&
      [[ "$(< "$build_marker")" == "$build_hash" ]]; then
    return 0
  fi
  rm -f "$build_marker" || return 1
  if [[ ! -f "$folder/.viper_extract_complete" ]] ||
      [[ "$(< "$folder/.viper_extract_complete")" != "$expected_hash" ]]; then
    rm -f "$folder/.viper_extract_complete" || return 1
    if [[ ! -f "$archive" ]]; then
      curl -fsSL -o "$archive" "$url" || {
        rm -f "$archive"
        return 1
      }
    fi
    local actual_hash
    actual_hash=$(sha256 "$archive")
    if [[ "$actual_hash" != "$expected_hash" ]]; then
      echo "Error: SHA256 mismatch for $archive."
      echo "  Expected: $expected_hash"
      echo "  Actual:   $actual_hash"
      rm -f "$archive"
      return 1
    fi
    mkdir -p "$folder" || return 1
    if [[ "$archive" == *.zip ]]; then
      unzip -qo "$archive" || return 1
    else
      tar -xf "$archive" --strip-components=1 -C "$folder" || return 1
    fi
    echo "$expected_hash" > "$folder/.viper_extract_complete" || return 1
  fi
  if [[ -n "$build_func" ]]; then
    pushd "$folder" > /dev/null || return 1
    $build_func || { popd > /dev/null; return 1; }
    popd > /dev/null
  fi
  echo "$build_hash" > "$build_marker" || return 1
  rm -f "$archive"
}

main "$@"
