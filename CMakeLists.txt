CMAKE_MINIMUM_REQUIRED(VERSION 3.11)

PROJECT(
  Plorth
  VERSION 2.0.0
  DESCRIPTION "Interpreter for Plorth programming language."
  HOMEPAGE_URL "https://github.com/plorth/interpreter"
  LANGUAGES C CXX
)

INCLUDE(CMakePackageConfigHelpers)
INCLUDE(FetchContent)

FETCHCONTENT_DECLARE(
  PlorthParser
  GIT_REPOSITORY
    https://github.com/plorth/parser.git
  GIT_TAG
    v2.3.0
)
FETCHCONTENT_DECLARE(
  NlohmannJson
  GIT_REPOSITORY
    https://github.com/nlohmann/json.git
  GIT_TAG
    v3.11.2
)
# TODO: Remove this. It appears that the JSON library also contains this.
FETCHCONTENT_DECLARE(
  OrderedMap
  GIT_REPOSITORY
    https://github.com/tessil/ordered-map.git
  GIT_TAG
    v1.1.0
)

FETCHCONTENT_MAKEAVAILABLE(PlorthParser)
FETCHCONTENT_MAKEAVAILABLE(NlohmannJson)
FETCHCONTENT_MAKEAVAILABLE(OrderedMap)

ADD_LIBRARY(
  plorth
  SHARED
  src/compiler.cpp
  src/context.cpp
  src/eval.cpp
  src/exec.cpp
  src/json.cpp
  src/runtime.cpp
  src/api/array.cpp
  src/api/boolean.cpp
  src/api/error.cpp
  src/api/global.cpp
  src/api/number.cpp
  src/api/object.cpp
  src/api/quote.cpp
  src/api/string.cpp
  src/api/symbol.cpp
  src/api/word.cpp
  src/value/array.cpp
  src/value/base.cpp
  src/value/boolean.cpp
  src/value/error.cpp
  src/value/null.cpp
  src/value/number.cpp
  src/value/object.cpp
  src/value/quote.cpp
  src/value/string.cpp
  src/value/symbol.cpp
  src/value/word.cpp
)

TARGET_COMPILE_OPTIONS(
  plorth
  PRIVATE
    -Wall -Werror
)

TARGET_COMPILE_FEATURES(
  plorth
  PRIVATE
    cxx_std_17
)

TARGET_INCLUDE_DIRECTORIES(
  plorth
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

TARGET_LINK_LIBRARIES(
  plorth
  PUBLIC
    NlohmannJson
    OrderedMap
    PlorthParser
)

INSTALL(
  TARGETS
    plorth
  EXPORT
    ${PROJECT_NAME}_Targets
  ARCHIVE DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION
    ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}
)

WRITE_BASIC_PACKAGE_VERSION_FILE(
  "${PROJECT_NAME}ConfigVersion.cmake"
  VERSION
    ${PROJECT_VERSION}
  COMPATIBILITY
    SameMajorVersion
)

INSTALL(
  TARGETS
    plorth
  LIBRARY DESTINATION
    lib
)

INSTALL(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include
  FILES_MATCHING PATTERN "*.hpp"
)

INSTALL(
  DIRECTORY ${CMAKE_SOURCE_DIR}/runtime/
  DESTINATION lib/plorth
  FILES_MATCHING PATTERN "*.plorth"
)
