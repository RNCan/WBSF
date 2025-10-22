cmake_minimum_required(VERSION 3.25)
project(WBSFGeomatic LANGUAGES CXX)

find_package(Boost CONFIG REQUIRED)
find_package(GDAL CONFIG REQUIRED)

file(GLOB_RECURSE WBSF_GEOMATIC_SRC *.cpp *.h *.hpp)
list(FILTER WBSF_GEOMATIC_SRC EXCLUDE REGEX "Projection.cpp$")	#exclude Projection.cpp

add_library(WBSFGeomatic STATIC
    ${WBSF_GEOMATIC_SRC}
)

target_link_libraries(WBSFGeomatic PUBLIC Boost::boost GDAL::GDAL)
target_include_directories(WBSFGeomatic PUBLIC ${PROJECT_SOURCE_DIR}/..)
