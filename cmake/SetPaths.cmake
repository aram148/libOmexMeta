
##########################################################################
# variables for building semsim dependencies
#

set(THIRD_PARTY_DIRS ${CMAKE_SOURCE_DIR}/third_party)

# google test source dir
set(GOOGLETEST_SOURCE ${THIRD_PARTY_DIRS}/googletest)

set(DEPENDENCY_INSTALL_FOLDER_NAME "install2")
set(DEPENDENCY_BUILD_FOLDER_NAME "build2")

# openssl - used for secure download of files from tinterweb
#set(OPENSSL_SOURCE_DIR ${THIRD_PARTY_DIRS}/openssl-cmake-1.0.1e-src)
#set(OPENSSL_BINARY_DIR ${OPENSSL_SOURCE_DIR}/${DEPENDENCY_BUILD_FOLDER_NAME}-${PLATFORM})
#set(OPENSSL_INSTALL_PREFIX ${OPENSSL_SOURCE_DIR}/${DEPENDENCY_INSTALL_FOLDER_NAME}-${PLATFORM})
#set(OPENSSL_INCLUDE_DIR ${OPENSSL_INSTALL_PREFIX}/include)
#set(OPENSSL_LIB_DIR ${OPENSSL_INSTALL_PREFIX}/lib)
#set(LIBCRYPTO_STATIC_LIBRARY ${OPENSSL_LIB_DIR}/libcrypto.a)
#set(SSL_STATIC_LIBRARY ${OPENSSL_LIB_DIR}/libssl.a)


# dependency package for libsbml. Even though libsbml is no longer a
# required dependency of libsemsim, some of these are used elsewhere
set(LIBSBML_DEPS_SOURCE_DIR ${THIRD_PARTY_DIRS}/libSBML-dependencies)
set(LIBSBML_DEPS_BINARY_DIR ${LIBSBML_DEPS_SOURCE_DIR}/${DEPENDENCY_BUILD_FOLDER_NAME}-${PLATFORM})
set(LIBSBML_DEPS_INSTALL_PREFIX ${LIBSBML_DEPS_SOURCE_DIR}/${DEPENDENCY_INSTALL_FOLDER_NAME}-${PLATFORM})
set(LIBSBML_DEPS_INCLUDE_DIR ${LIBSBML_DEPS_INSTALL_PREFIX}/include)
set(LIBSBML_DEPS_LIB_DIR ${LIBSBML_DEPS_INSTALL_PREFIX}/lib)


# libiconv - character encodings
set(ICONV_LIB_DIR "${LIBSBML_DEPS_INSTALL_PREFIX}/lib")
set(ICONV_INCLUDE_DIR "${LIBSBML_DEPS_INSTALL_PREFIX}/include")
set(ICONV_STATIC_LIBRARY ${ICONV_LIB_DIR}/libiconv.a)


set(LIBSBML_SOURCE_DIR ${THIRD_PARTY_DIRS}/libsbml-5.18.0)
set(LIBSBML_BINARY_DIR ${LIBSBML_SOURCE_DIR}/${DEPENDENCY_BUILD_FOLDER_NAME}-${PLATFORM})
set(LIBSBML_INSTALL_PREFIX ${LIBSBML_SOURCE_DIR}/${DEPENDENCY_INSTALL_FOLDER_NAME}-${PLATFORM})
set(LIBSBML_INCLUDE_DIR ${LIBSBML_INSTALL_PREFIX}/include)
set(LIBSBML_LIB_DIR ${LIBSBML_INSTALL_PREFIX}/lib)
set(LIBSBML_STATIC_LIBRARY ${LIBSBML_LIB_DIR}/libsbml-static.a)

set(ZIPPER_SOURCE_DIR ${THIRD_PARTY_DIRS}/zipper)
set(ZIPPER_BINARY_DIR ${ZIPPER_SOURCE_DIR}/${DEPENDENCY_BUILD_FOLDER_NAME}-${PLATFORM})
set(ZIPPER_INSTALL_PREFIX ${ZIPPER_SOURCE_DIR}/${DEPENDENCY_INSTALL_FOLDER_NAME}-${PLATFORM})
set(ZIPPER_LIB_DIR "${ZIPPER_INSTALL_PREFIX}/lib")
set(ZIPPER_INCLUDE_DIR "${ZIPPER_INSTALL_PREFIX}/include")
set(ZIPPER_STATIC_LIBRARY ${ZIPPER_LIB_DIR}/libZipper-static.a)


set(ZLIB_SOURCE_DIR ${THIRD_PARTY_DIRS}/zlib-1.2.11)
set(ZLIB_BINARY_DIR ${ZLIB_SOURCE_DIR}/${DEPENDENCY_BUILD_FOLDER_NAME}-${PLATFORM})
set(ZLIB_INSTALL_PREFIX ${ZLIB_SOURCE_DIR}/${DEPENDENCY_INSTALL_FOLDER_NAME}-${PLATFORM})
set(ZLIB_LIB_DIR "${ZLIB_INSTALL_PREFIX}/lib")
set(ZLIB_INCLUDE_DIR "${ZLIB_INSTALL_PREFIX}/include")
set(ZLIB_STATIC_LIBRARY ${ZLIB_LIB_DIR}/libz.a)

set(BZ2_INCLUDE_DIR "${LIBSBML_DEPS_LIB_DIR}/include")
set(BZ2_STATIC_LIBRARY ${LIBSBML_DEPS_LIB_DIR}/libbz2.a)

set(LIBCOMBINE_SOURCE_DIR ${THIRD_PARTY_DIRS}/libCombine-0.2.3)
set(LIBCOMBINE_BINARY_DIR ${LIBCOMBINE_SOURCE_DIR}/${DEPENDENCY_BUILD_FOLDER_NAME}-${PLATFORM})
set(LIBCOMBINE_INSTALL_PREFIX ${LIBCOMBINE_SOURCE_DIR}/${DEPENDENCY_INSTALL_FOLDER_NAME}-${PLATFORM})
set(LIBCOMBINE_LIB_DIR "${LIBCOMBINE_INSTALL_PREFIX}/lib")
set(LIBCOMBINE_INCLUDE_DIR "${LIBCOMBINE_INSTALL_PREFIX}/include")
set(LIBCOMBINE_STATIC_LIBRARY "${LIBCOMBINE_LIB_DIR}/libCombine-static.a")

set(LIBXML2_SOURCE_DIR ${THIRD_PARTY_DIRS}/libxml2)
set(LIBXML2_BINARY_DIR ${LIBXML2_SOURCE_DIR}/${DEPENDENCY_BUILD_FOLDER_NAME}-${PLATFORM})
set(LIBXML2_INSTALL_PREFIX ${LIBXML2_SOURCE_DIR}/${DEPENDENCY_INSTALL_FOLDER_NAME}-${PLATFORM})
set(LIBXML2_LIB_DIR "${LIBXML2_INSTALL_PREFIX}/lib")
set(LIBXML2_INCLUDE_DIR "${LIBXML2_INSTALL_PREFIX}/include/libxml2")
set(LIBXML2_STATIC_LIBRARY "${LIBXML2_LIB_DIR}/libxml2.a")


SET(STATIC_LIBRARIES
        ${LIBCRYPTO_STATIC_LIBRARY}
        ${SSL_STATIC_LIBRARY}
        ${ICONV_STATIC_LIBRARY}
        ${LIBSBML_STATIC_LIBRARY}
        ${ZIPPER_STATIC_LIBRARY}
        ${ZLIB_STATIC_LIBRARY}
        ${BZ2_STATIC_LIBRARY}
        ${LIBCOMBINE_STATIC_LIBRARY}
        ${LIBXML2_STATIC_LIBRARY})







