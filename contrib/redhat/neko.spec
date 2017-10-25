Name:           neko
Version:        0.6.3
Release:        1%{?dist}
Summary:        Neko Wallet
Group:          Applications/Internet
Vendor:         Neko
License:        GPLv3
URL:            https://www.neko.com
Source0:        %{name}-%{version}.tar.gz
BuildRoot:      %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRequires:  autoconf automake libtool gcc-c++ openssl-devel >= 1:1.0.2d libdb4-devel libdb4-cxx-devel miniupnpc-devel boost-devel boost-static
Requires:       openssl >= 1:1.0.2d libdb4 libdb4-cxx miniupnpc logrotate

%description
Neko Wallet

%prep
%setup -q

%build
./autogen.sh
./configure
make

%install
%{__rm} -rf $RPM_BUILD_ROOT
%{__mkdir} -p $RPM_BUILD_ROOT%{_bindir} $RPM_BUILD_ROOT/etc/neko $RPM_BUILD_ROOT/etc/ssl/nk $RPM_BUILD_ROOT/var/lib/nk/.neko $RPM_BUILD_ROOT/usr/lib/systemd/system $RPM_BUILD_ROOT/etc/logrotate.d
%{__install} -m 755 src/nekod $RPM_BUILD_ROOT%{_bindir}
%{__install} -m 755 src/neko-cli $RPM_BUILD_ROOT%{_bindir}
%{__install} -m 600 contrib/redhat/neko.conf $RPM_BUILD_ROOT/var/lib/nk/.neko
%{__install} -m 644 contrib/redhat/nekod.service $RPM_BUILD_ROOT/usr/lib/systemd/system
%{__install} -m 644 contrib/redhat/nekod.logrotate $RPM_BUILD_ROOT/etc/logrotate.d/nekod
%{__mv} -f contrib/redhat/nk $RPM_BUILD_ROOT%{_bindir}

%clean
%{__rm} -rf $RPM_BUILD_ROOT

%pretrans
getent passwd nk >/dev/null && { [ -f /usr/bin/nekod ] || { echo "Looks like user 'nk' already exists and have to be deleted before continue."; exit 1; }; } || useradd -r -M -d /var/lib/nk -s /bin/false nk

%post
[ $1 == 1 ] && {
  sed -i -e "s/\(^rpcpassword=MySuperPassword\)\(.*\)/rpcpassword=$(cat /dev/urandom | tr -dc 'a-zA-Z0-9' | fold -w 32 | head -n 1)/" /var/lib/nk/.neko/neko.conf
  openssl req -nodes -x509 -newkey rsa:4096 -keyout /etc/ssl/nk/neko.key -out /etc/ssl/nk/neko.crt -days 3560 -subj /C=US/ST=Oregon/L=Portland/O=IT/CN=neko.nk
  ln -sf /var/lib/nk/.neko/neko.conf /etc/neko/neko.conf
  ln -sf /etc/ssl/nk /etc/neko/certs
  chown nk.nk /etc/ssl/nk/neko.key /etc/ssl/nk/neko.crt
  chmod 600 /etc/ssl/nk/neko.key
} || exit 0

%posttrans
[ -f /var/lib/nk/.neko/addr.dat ] && { cd /var/lib/nk/.neko && rm -rf database addr.dat nameindex* blk* *.log .lock; }
sed -i -e 's|rpcallowip=\*|rpcallowip=0.0.0.0/0|' /var/lib/nk/.neko/neko.conf
systnktl daemon-reload
systnktl status nekod >/dev/null && systnktl restart nekod || exit 0

%preun
[ $1 == 0 ] && {
  systnktl is-enabled nekod >/dev/null && systnktl disable nekod >/dev/null || true
  systnktl status nekod >/dev/null && systnktl stop nekod >/dev/null || true
  pkill -9 -u nk > /dev/null 2>&1
  getent passwd nk >/dev/null && userdel nk >/dev/null 2>&1 || true
  rm -f /etc/ssl/nk/neko.key /etc/ssl/nk/neko.crt /etc/neko/neko.conf /etc/neko/certs
} || exit 0

%files
%doc COPYING
%attr(750,nk,nk) %dir /etc/neko
%attr(750,nk,nk) %dir /etc/ssl/nk
%attr(700,nk,nk) %dir /var/lib/nk
%attr(700,nk,nk) %dir /var/lib/nk/.neko
%attr(600,nk,nk) %config(noreplace) /var/lib/nk/.neko/neko.conf
%attr(4750,nk,nk) %{_bindir}/neko-cli
%defattr(-,root,root)
%config(noreplace) /etc/logrotate.d/nekod
%{_bindir}/nekod
%{_bindir}/nk
/usr/lib/systemd/system/nekod.service

%changelog
* Thu Aug 31 2017 Aspanta Limited <info@aspanta.com> 0.6.3
- There is no changelog available. Please refer to the CHANGELOG file or visit the website.
