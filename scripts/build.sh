# Конфигурация
PROJECT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BUILD_DIR="${PROJECT_DIR}/build
CLEAN_BUILD=false


if [ "$CLEAN_BUILD" = true ]; then
  echo "Очистка предыдущей сборки..."
  rm -rf "${BUILD_DIR}"
fi

mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

cmake -G Ninja ..

ninja
