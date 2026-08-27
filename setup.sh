#!/bin/bash
set -o errexit
set -o pipefail
DIRECTORY=""
ROOT=""
CACHE_NAME=""
SETUP_HASH=""
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
  check_cache "viper" || exit 0
  add_dependency "doctest-2.4.12" \
    "https://github.com/doctest/doctest/archive/refs/tags/v2.4.12.zip" \
    "7a7afb5f70d0b749d49ddfcb8a454299a8fcd53e9db9c131abe99b456e88a1fe"
  add_dependency "sqlite-amalgamation-3510200" \
    "https://www.sqlite.org/2026/sqlite-amalgamation-3510200.zip" \
    "6e2a845a493026bdbad0618b2b5a0cf48584faab47384480ed9f592d912f23ec" \
    "build_sqlite"
  add_dependency "openssl-3.6.0" \
    "https://github.com/openssl/openssl/releases/download/openssl-3.6.0/openssl-3.6.0.tar.gz" \
    "b6a5f44b7eb69e3fa35dbf15524405b44837a481d43d81daddde3ff21fcbb8e9" \
    "build_openssl"
  add_dependency "mariadb-connector-c-3.4.9" \
    "https://github.com/mariadb-corporation/mariadb-connector-c/archive/refs/tags/v3.4.9.zip" \
    "2342f6e58907f7431b5ccafb8b8e744b6b0e64174d72395d2330576b8a535fb6" \
    "build_mariadb"
  install_dependencies || return 1
  commit
}

build_sqlite() {
  gcc -c -O2 -o sqlite3.lib -DSQLITE_USE_URI=1 -fPIC sqlite3.c || return 1
}

build_openssl() {
  local cores
  cores=$(get_core_count)
  popd > /dev/null
  mv "openssl-3.6.0" "openssl-3.6.0-build"
  pushd "openssl-3.6.0-build" > /dev/null
  export LDFLAGS=-ldl
  ./config no-shared threads -fPIC -ldl --prefix="$ROOT/openssl-3.6.0" ||
    return 1
  make -j "$cores" || return 1
  make test || return 1
  make install || return 1
  unset LDFLAGS
  popd > /dev/null
  rm -rf "openssl-3.6.0-build"
  pushd "openssl-3.6.0" > /dev/null
}

build_mariadb() {
  local cores
  cores=$(get_core_count)
  export OPENSSL_ROOT_DIR="$ROOT/openssl-3.6.0"
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=./mariadb \
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

check_cache() {
  CACHE_NAME="$1"
  SETUP_HASH=$(sha256 "$DIRECTORY/setup.sh")
  if [[ -f "cache_files/$CACHE_NAME.txt" ]]; then
    local cached_hash
    cached_hash=$(< "cache_files/$CACHE_NAME.txt")
    if [[ "$SETUP_HASH" == "$cached_hash" ]]; then
      return 1
    fi
  fi
  return 0
}

commit() {
  if [[ ! -d "cache_files" ]]; then
    mkdir -p cache_files || return 1
  fi
  echo "$SETUP_HASH" > "cache_files/$CACHE_NAME.txt"
}

add_dependency() {
  local name="$1"
  local url="$2"
  local hash="$3"
  local build="${4:-}"
  DEPENDENCIES+=("$name|$url|$hash|$build")
}

install_dependencies() {
  for dep in "${DEPENDENCIES[@]}"; do
    IFS='|' read -r name url hash build <<< "$dep"
    download_and_extract "$name" "$url" "$hash" "$build" || return 1
  done
}

download_and_extract() {
  local folder="$1"
  local url="$2"
  local expected_hash="$3"
  local build_func="$4"
  local archive="${url##*/}"
  if [[ -d "$folder" ]]; then
    return 0
  fi
  if [[ ! -f "$archive" ]]; then
    curl -fsSL -o "$archive" "$url" || return 1
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
    unzip -q "$archive" -d "$folder" || { rm -rf "$folder"; return 1; }
  else
    tar -xf "$archive" -C "$folder" || { rm -rf "$folder"; return 1; }
  fi
  flatten_directory "$folder"
  if [[ -n "$build_func" ]]; then
    pushd "$folder" > /dev/null
    $build_func || { popd > /dev/null; return 1; }
    popd > /dev/null
  fi
  rm -f "$archive"
}

flatten_directory() {
  local folder="$1"
  local dir_count=0
  local file_count=0
  local single_dir=""
  for d in "$folder"/*/; do
    if [[ -d "$d" ]]; then
      ((++dir_count))
      single_dir="$d"
    fi
  done
  for f in "$folder"/*; do
    if [[ -f "$f" ]]; then
      ((++file_count))
    fi
  done
  if [[ "$dir_count" -eq 1 ]] && [[ "$file_count" -eq 0 ]]; then
    shopt -s dotglob
    mv "$single_dir"* "$folder/" 2>/dev/null || true
    shopt -u dotglob
    rmdir "$single_dir" 2>/dev/null || true
  fi
}

main "$@"
