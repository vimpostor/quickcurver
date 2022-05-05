FROM vimpostor/arch-qt6
ADD . /build
WORKDIR /build
RUN pacman -Syu --noconfirm doxygen
RUN scripts/build.sh
