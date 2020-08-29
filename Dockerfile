FROM vimperator97/qt5
ADD . /build
WORKDIR /build
RUN scripts/build.sh
