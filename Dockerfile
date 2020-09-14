# ciaranwelsh/ubuntu-base:latest suppies a lot for the build enviornment

FROM ciaranwelsh/libomexmeta-build-env

# checkout libomexmeta
# this is the build environment only
ENV install_prefix "/libOmexMeta/install-docker"
RUN git clone https://github.com/sys-bio/libOmexMeta.git \
    && cd libOmexMeta \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_INSTALL_PREFIX="$install_prefix" -DVCPKG_ROOT="/vcpkg" .. \
    && make -j 8 \
    && make install


