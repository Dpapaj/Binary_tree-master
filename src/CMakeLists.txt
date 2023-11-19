set(SOURCES tree.c data.c)

set(HEADER_LIST 
    include/tree.h
    include/data.h
  )

set(LIBNAME "binaryTree")

add_library(${LIBNAME} ${SOURCES} ${HEADER_LIST})

include(FetchContent)
FetchContent_Declare(
  myMalloc
  GIT_REPOSITORY https://mymalloc:8zj7GZZNmWyLGgQSyzsN@git.vyuka.fai.utb.cz/ads/common/mymalloc.git
)

FetchContent_MakeAvailable(myMalloc)

FetchContent_Declare(
  ioutils
  GIT_REPOSITORY https://ioutils:aFiG6LqzyabnzhxzbhNC@git.vyuka.fai.utb.cz/ads/common/ioutils.git
)

FetchContent_MakeAvailable(ioutils)

target_link_libraries(${LIBNAME} PUBLIC myMalloc ioutils)
target_include_directories(${LIBNAME} PUBLIC include)
