FROM vimpostor/arch-qt6
ADD . /build
WORKDIR /build
RUN pacman -Syu --noconfirm doxygen imagemagick binutils curl
RUN scripts/build.sh
