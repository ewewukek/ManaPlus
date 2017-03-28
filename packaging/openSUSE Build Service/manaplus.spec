Name:		manaplus
Version:	1.1.6.26
Release:	1
Summary:	A client for Evol Online and The Mana World: 2D MMORPG
Group:		Games/Other
License:	GPLv2+
Url:		http://manaplus.evolonline.org/
Source0:	http://download.evolonline.org/manaplus/download/%{version}/%{name}-%{version}.tar.bz2
BuildRoot:	%{_topdir}/%{name}-%{version}-root

BuildRequires:	gcc-c++
BuildRequires:	make
BuildRequires:	SDL-devel
BuildRequires:	SDL_image-devel
BuildRequires:	SDL_mixer-devel
Buildrequires:	SDL_net-devel
BuildRequires:	SDL_ttf-devel
BuildRequires:	SDL_gfx-devel
BuildRequires:	libcurl-devel
BuildRequires:	libxml2-devel
BuildRequires:	libpng-devel
BuildRequires:	gettext-devel

Provides:	evolonline-client = %{version}-%{release}
Provides:	manaworld-client = %{version}-%{release}

%description
ManaPlus is extended client for Evol Online, The Mana World and similar
servers based on eAthena fork.
As a 2D style game, Evol Online aims to create a friendly environment where 
people can escape reality and interact with others while enjoying themselves 
through a fantasy style game.
The Mana World (TMW) is a serious effort to create an innovative free and 
open source MMORPG. TMW uses 2D graphics and aims to create a large and 
diverse interactive world.

%prep
%setup -q

%build
%configure --host=
%{__make}

%install
rm -rf %{buildroot}
%makeinstall

%find_lang %{name}

%clean
rm -rf %{buildroot}

%files -f %{name}.lang
%attr(-, root, root) %doc AUTHORS ChangeLog COPYING docs/*.txt NEWS README
%attr(0755, root, root) %{_bindir}/%{name}
%attr(-, root, root) %{_datadir}/%{name}
%attr(-, root, root) %{_datadir}/pixmaps/%{name}.png
%attr(-, root, root) %{_datadir}/applications/%{name}.desktop
%attr(-, root, root) %{_mandir}/man6/%{name}*


%changelog
* Sat Jul 02 2011 Andrei Karas <akaras@inbox.ru> 1.1.6.26
- Add missing dependency SDL_image and compilation tools

* Fri May 27 2011 Maarten Vanraes <maarten.vanraes@gmail.com> 1.1.5.15
- Generalise spec file to work for multiple rpm-based distros

* Sun Apr 17 2011 alien <alien> 1.1.4.17-1.mga1
+ Revision: 87336
- upgrade to 1.1.4.17

* Fri Apr 08 2011 alien <alien> 1.1.4.3-1.mga1
+ Revision: 82020
- Update to new version 1.1.4.3
- manaplus is now primarily for Evol Online, updating Summary and Description and add Provides
- suggests mumble

* Sat Feb 26 2011 alien <alien> 1.1.2.20-1.mga1
+ Revision: 61040
- Fix BuildRequires
- Fix configure part
- Remove unneeded mv files
- Remove BuildRoot
- use tmw spec file as basis
- imported package manaplus


* Sun Jan 31 2010 Jérôme Brenier <incubusss@mandriva.org> 0.0.29.1-3mdv2010.1
+ Revision: 498772
- add version and release for Provides

* Tue Jan 05 2010 Jérôme Brenier <incubusss@mandriva.org> 0.0.29.1-2mdv2010.1
+ Revision: 486410
- use _gamesbindir and _gamesdatadir
- Suggests: tmwmusic

* Tue Jan 05 2010 Jérôme Brenier <incubusss@mandriva.org> 0.0.29.1-1mdv2010.1
+ Revision: 486388
- add some provides on themanaworld and manaworld
- import tmw
