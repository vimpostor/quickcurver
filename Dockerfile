FROM base/archlinux
ADD . /build
WORKDIR /build
RUN scripts/installDependencies.sh
# Do not require the renameat2 syscall
RUN strip --remove-section=.note.ABI-tag /usr/lib64/libQt5Core.so.5
RUN scripts/build.sh
