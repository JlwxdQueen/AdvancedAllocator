#!/bin/bash
set -euo pipefail

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
PROJECT_DIR=$(dirname "$SCRIPT_DIR")
BUILD_DIR="${PROJECT_DIR}/build"

CLEAN_BUILD=false
if [[ "$#" -gt 0 && "$1" == "--clean" ]]; then
  CLEAN_BUILD=true
fi

if [ "$CLEAN_BUILD" = true ]; then
  echo "Очистка предыдущей сборки..."
  rm -rf "${BUILD_DIR}"
fi

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

if ! command -v cmake &> /dev/null; then
  echo "Ошибка: cmake не установлен. Установите cmake и попробуйте снова."
  exit 1
fi
if ! command -v ninja &> /dev/null; then
  echo "Ошибка: ninja не установлен. Установите ninja и попробуйте снова."
  exit 1
fi

echo "Генерация сборки в ${BUILD_DIR}..."
cmake -G Ninja "$PROJECT_DIR" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_FLAGS="-Os -ffunction-sections -fdata-sections" \
  -DCMAKE_CXX_FLAGS="-Os -ffunction-sections -fdata-sections"

echo "Сборка проекта..."
ninja

echo "Сборка завершена успешно!"