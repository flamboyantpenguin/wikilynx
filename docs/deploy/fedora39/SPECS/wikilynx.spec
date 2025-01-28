Name:		wikilynx
Version:	1.5.6
Release:    1%{?dist}
Summary:	A simple Qt C++ app to play Wikipedia speed-run the right way
URL:		https://github.com/flamboyantpenguin/wikilynx
License:	MIT
Source0:	https://github.com/flamboyantpenguin/wikilynx/archive/refs/tags/v1.5.6.tar.gz
Source1:	https://repo.dawn.org.in/rpm/dawn.repo
BuildRequires:	cmake, qt6-qtwebengine-devel, qt6-qtmultimedia-devel
Requires:	qt6-qtwebengine, qt6-qtsvg, qt6-qtmultimedia

%description
Navigate your way through articles in Wikipedia through hyperlinks.

%prep
%setup -q


%build
mkdir -p build
sed 's/::checkStateChanged/::stateChanged/g' -i ./wikiLYNX/about.cpp
sed 's/::checkStateChanged/::stateChanged/g' -i ./wikiLYNX/welcome.cpp
sed 's/::checkStateChanged/::stateChanged/g' -i ./wikiLYNX/leveleditor.cpp
sed 's/::checkStateChanged/::stateChanged/g' -i ./wikiLYNX/levelmanager.cpp
cd build
cmake ../wikiLYNX -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=MinSizeRel   # Set install prefix
make

%install

# bin
cd build
make DESTDIR=%{buildroot} install  # Use make install

# Repo
install -D -m 0644 %{SOURCE1} %{buildroot}/etc/yum.repos.d/dawn.repo

%files
/usr/bin/wikilynx
/usr/share/licenses/wikilynx/LICENSE
/usr/share/licenses/wikilynx/COPYING
/usr/share/licenses/wikilynx/COPYING.QtWebEngine
/usr/share/licenses/wikilynx/NotoSans_OFL.txt
/usr/share/licenses/wikilynx/Comfortaa_OFL.txt
/usr/share/licenses/wikilynx/CourierPrime_OFL.txt
/usr/share/licenses/wikilynx/LICENSE-MaterialIcons.txt
/usr/share/applications/in.org.dawn.wikilynx.desktop
/usr/share/icons/hicolor/scalable/apps/in.org.dawn.wikilynx.svg
/usr/share/metainfo/in.org.dawn.wikilynx.appdata.xml
%config(noreplace) /etc/yum.repos.d/dawn.repo

%changelog
* Fri Jan 31 2025 Penguin <penguin@dawn.org.in> - 1.5.6-1
- Release 1.5.6-1
- Repository now supports AppStream Metainfo

* Fri Jan 03 2025 Penguin <penguin@dawn.org.in> - 1.5.5-3
- Release 1.5.5-3
- Fixes for bundling flatpak

* Thu Jan 02 2025 Penguin <penguin@dawn.org.in> - 1.5.5-2
- Release 1.5.5-2
- AppStream Fixes

* Thu Jan 02 2025 Penguin <penguin@dawn.org.in> - 1.5.5-1
- Release 1.5.5

* Wed Dec 18 2024 Penguin <penguin@dawn.org.in> - 1.5.0-1
- Release 1.5.0

* Wed Nov 06 2024 Penguin <penguin@dawn.org.in> - 1.3.0-1
- Release 1.3.0

* Sat Nov 02 2024 Penguin <penguin@dawn.org.in> - 1.2.8-1
- Release 1.2.8

* Tue Oct 29 2024 Penguin <penguin@dawn.org.in> - 1.2.7-0
- Release 1.2.7

* Sat Oct 19 2024 Penguin <penguin@dawn.org.in> - 1.2.6-0
- Release 1.2.6

* Sat Oct 19 2024 Penguin <penguin@dawn.org.in> - 1.2.5-2
- Added repo
- Added Project URL

* Sat Oct 19 2024 Penguin <penguin@dawn.org.in> - 1.2.5-1
- Initial release.
