FROM vimpostor/arch-qt6
ADD . /build
WORKDIR /build
RUN scripts/build.sh
