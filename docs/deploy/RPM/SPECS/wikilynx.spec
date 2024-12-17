Name:		wikilynx
Version:	1.5.0
Release:	1%{?dist}
Summary:	A simple Qt C++ app to play Wikipedia speed-run the right way
URL:		https://github.com/flamboyantpenguin/wikilynx
License:	MIT
Source0:	https://github.com/flamboyantpenguin/wikilynx/archive/refs/tags/v1.5.0.tar.gz
Source1:	https://repo.pcland.co.in/rpm/dawn.repo
BuildRequires:	cmake, qt6-qtwebengine-devel
Requires:	qt6-qtwebengine, qt6-qtsvg

%description
Navigate your way through articles in Wikipedia through hyperlinks.

%prep
%setup -q


%build
mkdir -p build
cd build
cmake ../wikiLYNX -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=MinSizeRel   # Set install prefix
make

%install

# bin
cd build
make DESTDIR=%{buildroot} install  # Use make install

# GUI
install -D -m 0644 ./assets/images/wikiLYNX_logo.svg %{buildroot}/usr/share/icons/hicolor/scalable/apps/wikiLYNX_logo.svg
install -D -m 0644 ../docs/desktop/wikilynx.desktop %{buildroot}/usr/share/applications/wikilynx.desktop
install -D -m 0644 ../docs/desktop/wikilynx.appdata.xml %{buildroot}/usr/share/metainfo/wikilynx.appdata.xml


# Docs
install -D -m 0644 ../LICENSE %{buildroot}/usr/share/licenses/wikilynx/LICENSE
install -D -m 0644 ../COPYING %{buildroot}/usr/share/licenses/wikilynx/COPYING
install -D -m 0644 ../COPYING.QtWebEngine %{buildroot}/usr/share/licenses/wikilynx/COPYING.QtWebEngine
install -D -m 0644 ../docs/license/NotoSans_OFL.txt %{buildroot}/usr/share/licenses/wikilynx/NotoSans_OFL.txt
install -D -m 0644 ../docs/license/CourierPrime_OFL.txt %{buildroot}/usr/share/licenses/wikilynx/CourierPrime_OFL.txt
install -D -m 0644 ../docs/license/LICENSE-MaterialIcons.txt %{buildroot}/usr/share/licenses/wikilynx/LICENSE-MaterialIcons.txt

# Repo
install -D -m 0644 %{SOURCE1} %{buildroot}/etc/yum.repos.d/dawn.repo

%files
/usr/bin/wikilynx
/usr/share/licenses/wikilynx/LICENSE
/usr/share/licenses/wikilynx/COPYING
/usr/share/licenses/wikilynx/COPYING.QtWebEngine
/usr/share/licenses/wikilynx/NotoSans_OFL.txt
/usr/share/licenses/wikilynx/CourierPrime_OFL.txt
/usr/share/licenses/wikilynx/LICENSE-MaterialIcons.txt
/usr/share/applications/wikilynx.desktop
/usr/share/pixmaps/wikiLYNX_logo.svg
%config(noreplace) /etc/yum.repos.d/dawn.repo

%changelog
* Wed Nov 06 2024 Penguin <penguin@pcland.co.in> - 1.3.0-1
- Release 1.3.0

* Sat Nov 02 2024 Penguin <penguin@pcland.co.in> - 1.2.8-1
- Release 1.2.8

* Tue Oct 29 2024 Penguin <penguin@pcland.co.in> - 1.2.7-0
- Release 1.2.7

* Sat Oct 19 2024 Penguin <penguin@pcland.co.in> - 1.2.6-0
- Release 1.2.6

* Sat Oct 19 2024 Penguin <penguin@pcland.co.in> - 1.2.5-2
- Added repo
- Added Project URL

* Sat Oct 19 2024 Penguin <penguin@pcland.co.in> - 1.2.5-1
- Initial release.
