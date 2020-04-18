FROM archlinux/base
ADD . /build
WORKDIR /build
RUN scripts/installDependencies.sh
RUN scripts/build.sh
