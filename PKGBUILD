# Maintainer: Magnus Groß magnus.gross21@gmail.com
_pkgname=quickcurver
pkgname="$_pkgname"-git
pkgver=VERSION
pkgrel=1
pkgdesc="Qt Material design implementation of Achtung die Kurve with online multiplayer"
arch=('i686' 'x86_64')
url="https://github.com/magnus-gross/$_pkgname"
license=('GPL3')
depends=('qt5-base>=5.7 qt5-declarative qt5-svg qt5-quickcontrols qt5-quickcontrols2 qt5-graphicaleffects')
makedepends=('git make gcc sed')
optdepends=()
source=("git+https://github.com/magnus-gross/$_pkgname.git")
md5sums=('SKIP') #autofill using updpkgsums

build() {
  cd "$srcdir/$_pkgname"
  ./build.sh
}

pkgver() {
  cd "$pkgname"
  git describe --long | sed 's/\([^-]*-g\)/r\1/;s/-/./g'
}

check() {
  ls "$srcdir/_pkgname/build/QuickCurver"
}

package() {
  cp "$srcdir/_pkgname/build/QuickCurver" "$pkgdir/usr/bin/_pkgname"
}
