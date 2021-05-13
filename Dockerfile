FROM vimpostor/arch-qt5
ADD . /build
WORKDIR /build
RUN scripts/build.sh
