=========================================================================
Instalacija toolchain-a, tftp-a, nfs servera i drugo na host racunar
=========================================================================
sudo apt-get install gcc-arm-linux-gnueabihf
sudo apt-get install tftpd-hpa
sudo apt-get install nfs-kernel-server
(potrebno za svaki terminal pokrenuti ponovo komande koje slede...)
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
priprema usb2serial na host PC (u VM):
	podešavanje USB settings u VM tako da se usb2serial prijavi i na VM
	sudo apt-get install picocom
	picocom -b 115200 ttyUSB0

=========================================================================
Przenje image-a root fs-a i kernela na sd karticu
=========================================================================
sdcard.img - image sd kartice koji je dovoljno naprziti na karticu:
	1) (nije potrebno ako se przi image) formatiranje sd kartice:
		a. Windows
			i. Preuzeti alat ya formatiranje sa https://www.sdcard.org/downloads/formatter_4/eula_windows/
			ii. Instalirati i pokrenuti alat na racunaru
			iii. Postaviti "FORMAT SIZE ADJUSTMENT" opciju na "ON" u "Options" meniju
			iv. Proveriti da ubacena kartica odgovara onoj odabranoj u alatu
			v. Kliknite "Format" dugme
		b. Linux
			i. Predlazemo koriscenje alata gparted (ili command line verziju parted)
			ii. Formatirajte ceo disk kao FAT
	2) przenje sd kartice
		a. Windows
			i. Preuzeti Win32DiskImager sa http://downloads.sourceforge.net/project/win32diskimager/Archive/Win32DiskImager-0.9.5-install.exe?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fwin32diskimager%2F&ts=1469553342&use_mirror=netix
			ii. Instalirati i pokrenuti alat na racunaru
			iii. Odaberite image datoteku koju zelite da naprzite na karticu
			iv. Proveriti da ubacena kartica odgovara onoj odabranoj u alatu
			v. Kliknite na dugme "Write"
		b. Linux
			i. df -h
			ii. umount /dev/mmcblk0p1, ako je mmcblk0p1 prikljucena sd kartica, particija 1
			iii. umount /dev/mmcblk0p2, ako je mmcblk0p2 prikljucena sd kartica, particija 2
			iv. dd bs=4M status=progress if=sdcard.img of=/dev/mmcblk0, ako je sdd1 prikljucena sd kartica, a sdcard.img image koji treba naprziti
			v. sudo sync
		c. rpi
			tftp 0x01000000 sdcard.img
			tftp 0x02000000 uboot.env
			mmc rescan; mmc erase 0x0 0x4c002; mmc write 0x01000000 0x0 0x4C002; ls mmc 0:1; ls mmc 0:2; fatwrite mmc 0:1 0x02000000 uboot.env $filesize

=========================================================================
Podesavanja za boot preko NFS (OUTDATED PATHS)
=========================================================================
u u-boot folderu se nalazi:
	u-boot.bin / image u-boot-a, da bi se pokrenuo treba ga kopirati na sd karticu rpi-a na boot particiju i u config.txt fajlu dodati liniju "kernel=u-boot.bin"
	Kada se upali rpi, treba prekinuti timer koji pocne da odbrojava pritiskom na enter i ulazi se u u-boot.

u kernelAndDtb folderu se nalaze:
	zImage - image kernela
	bcm2709-rpi-2-b.dtb - device tree blob koji se koristi uz kernel

	Ove fajlove je potrebno podeliti preko tftp servera kako bi mogli da se prebace na rpi. Koristimo tftpd-hpa server.
	Instalira se komandom
		"sudo apt-get install tftpd-hpa".
	Ukoliko se nista ne menja folder koji se deli na mrezi je
		/var/lib/tftpboot.
	U taj folder treba prebaciti fajlove (image i device tree blob)
	Restartovati server komandom
		"sudo service tftpd-hpa restart".
	Fajlovi se u pokrenuti u-boot prebaciju komandama:
		setenv ipaddr <ip adresa> - ip koji dodeljujemo ploci, bilo koji slobodan ip na mrezi
		setenv serverip <ip adresa> - ip adresa racunara na kom se nalazi tftp server
		setenv bootargs "root=/dev/nfs rw ip=<ip adresa rpi-a> console=ttyAMA0 nfsroot=<ip adresa racunara na kom je nfs server>:<putanja do rfs-a na racunaru>"
		saveenv
		tftp 0x1000000 zImage - download kernela
		tftp 0x2000000 bcm2709-rpi-2-b.dtb - download dtb-a
		bootz 0x1000000 - 0x2000000

	boot se kasnije moze automatizovati	
		setenv bootcmd "tftp 0x1000000 zImage; tftp 0x2000000 bcm2709-rpi-2-b.dtb; bootz 0x1000000 - 0x2000000"

u folderu rootfs se nalaze: rootfs.ext2, rootfs.tar i rootfs.tar.bz2
	opcija 1: rootfs.tar.bz2 - kompresovana verzija rfs-a, potrebno je raspakovati i dobijeni folder exportovati preko nfs servera pre pokretanja
		"sudo tar xvjf rootfs.tar.bz2 [-C <putanja do direktorijuma gde ce se raspakovati>]"
	opcija 2: rootfs.ext2 - image koji se može pržiti i na karticu, pre pokretanja ga treba mountovati na neki folder koji se exportuje nfs serverom
		"mount rootfs.ext2 <putanja do direktorijuma gde ce se mountovati>"

	Koristimo nfs-kernel-server. Instalira se komandom
		"sudo apt-get install nfs-kernel-server".
	Posle toga dodati lokacije koje se exportuju u fajl /etc/exports u formatu
		"<putanja koja se exportuje> <ip adresa rpi-a>(rw,no_root_squash,no_subtree_check)"
	Restartovati nfs server komandom
		"sudo /etc/init.d/nfs-kernel-server restart"

=========================================================================
Przenje na sd karticu
=========================================================================
<TBD>
1) u-boot
2) kernel i dtb
3) rootfs
4) podesavanja
	a.
	b.
	c.


=========================================================================
Detaljni opis pojedinacnih fajlova:
=========================================================================

sdcard.img.tar.bz2 - kompresovana verzija image-a sd kartice koji je dovoljno naprziti na karticu po opisanom postupku

rpi-firmware/ - direktorijum sa datotekama neophodnim za boot, treba da se nalaze na boot particiji SD kartice

u-boot.bin - image u-boot-a, da bi se pokrenuo treba ga kopirati na sd karticu rpi-a na boot particiju i u config.txt fajlu dodati liniju "kernel=u-boot.bin"
	Kada se upali rpi, treba prekinuti timer koji pocne da odbrojava pritiskom na enter i ulazi se u u-boot.
uboot.env - bin fajl sa opisom env varijabli u-boot-a, treba da se nalazi na sd kartici, pored u-boot.bin i u tom slucaju ce ga u-boot koristiti za pocetnu konfiguraciju, a ako ne postoji koristice default.

zImage - image kernela
bcm2709-rpi-2-b.dtb - device tree blob koji se koristi uz kernel
	Ove fajlove je potrebno podeliti preko tftp servera kako bi mogli da se prebace na rpi. Koristimo tftpd-hpa server. Instalira se sa "sudo apt-get install tftpd-hpa". Ukoliko se nista ne menja folder koji se deli na mrezi je /var/lib/tftpboot. U taj folder treba prebaciti fajlove i restartovati server sa komandom "sudo service tftpd-hpa restart".
	Fajlovi se u pokrenuti u-boot prebaciju komandama.
	setenv ipaddr <ip adresa> - ip koji dodeljujemo ploci, bilo koji slobodan ip na mrezi
	setenv serverip <ip adresa> - ip adresa racunara na kom se nalazi tftp server
	setenv bootargs "root=/dev/nfs rw ip=<ip adresa rpi-a> console=ttyAMA0 nfsroot=<ip adresa racunara na kom je nfs server>:<putanja do rfs-a na racunaru>"
	saveenv
	tftp 0x1000000 zImage - download kernela
	tftp 0x2000000 bcm2709-rpi-2-b.dtb - download dtb-a
	bootz 0x1000000 - 0x2000000

	boot se kasnije moze automatizovati	
	setenv bootcmd "tftp 0x1000000 zImage; tftp 0x2000000 bcm2709-rpi-2-b.dtb; bootz 0x1000000 - 0x2000000"

rootfs.ext2.tar.bz2 - kompresovana verzija image-a root fs, pre pokretanja ga treba mountovati na neki folder koji se exportuje sa nfs serverom
rootfs.tar.bz2 - kompresovana verzija rfs-a, potrebno je raspakovati i taj folder exportovati preko nfs servera pre pokretanja
	raspakovan root fs se vec nalazi na putanji ../../modules/nfsroot

	Koristimo nfs-kernel-server. Instalira se sa "sudo apt-get install nfs-kernel-server". Posle toga dodati lokacije koje se exportuju u fajl /etc/exports u formatu
"<putanja koja se exportuje> <ip adresa rpi-a>(rw,no_root_squash,no_subtree_check)"
	Restartovati nfs server sa komandom "sudo /etc/init.d/nfs-kernel-server restart"

src/rootfs/ - minimalni rootfs (busybox)

=========================================================================
Bildovanje kernela
=========================================================================
prereq:
	sudo apt-get install flex
	sudo apt-get install bison
	sudo apt-get install libssl-dev
	sudo apt-get install qtbase5-dev
	sudo apt-get install g++

git clone -b rpi-5.10.y --single-branch https://github.com/raspberrypi/linux --depth=1
git branch 5.10.y 8e1110a580887f4b82303b9354c25d7e2ff5860e
git checkout 5.10.y.

cd linux
make bcm2709_defconfig
make

=========================================================================
Bildovanje minimalnog rootfs-a
=========================================================================
wget http://www.busybox.net/downloads/busybox-1.33.1.tar.bz2
tar xvjf busybox-1.33.1.tar.bz2 <direktorijum gde ce biti raspakovano>
opcija 1:
    make defconfig
    make menuconfig (build with static libraries, install directory nfsroot)
opcija 2:
    iskoristiti busybox-1.33.1.config
make
make install

U nfsroot folderu napraviti foldere proc/ sys/ etc/init.d
U etc/ folderu napraviti fajl inittab i popuniti ga sa 
::sysinit:/etc/init.d/rcS
::askfirst:/bin/sh

da bi se izbegla greska "/bin/sh: can't access tty; job control turned off"
treba pisati ttyAMA0::askfirst:/bin/sh, ali tada mora da postoji /dev
sa console i ttyAMA0, kao sto mora i za slucaj da se radi sa initramfs
mknod dev/console c 5 1
mknod dev/ttyAMA0 c 204 64

U etc/init.d/ folderu napraviti fajl rcS i popuniti ga sa
#!/bin/sh
mount -t proc none /proc
mount -t sysfs none /sys

za initramfs, u rfs treba preimenovati linuxrc u init i
CONFIG_INITRAMFS_SOURCE postaviti na putanju do napravljenog rfs u
bootargs treba postaviti samo parametre rw i console(=ttyAMA0)

=========================================================================
Bildovanje u-boot-a
=========================================================================
git clone git://git.denx.de/u-boot.git
make rpi_2_defconfig
make all
	branch:
		v2021.07
	u trenutku build-a:
		(2021.07) 840658b093976390e9537724f802281c9c8439f5

=========================================================================
Bildovanje rootfs (buildroot)
=========================================================================
<TBD>

=========================================================================
Bildovanje toolchain-a (buildroot)
=========================================================================
<TBD>

=========================================================================
Bildovanje kernel-a, rootfs-a i toolchain-a (buildroot)
=========================================================================
prereq:
	sudo apt-get install device-tree-compiler

verzija:
	2021.08.x

kernel:
	custom
	https://github.com/raspberrypi/linux
	branch:
		rpi-5.10.y
	u trenutku build-a:
		(5.10.110) 8e1110a580887f4b82303b9354c25d7e2ff5860e

firmware:
	https://github.com/raspberrypi/firmware
	u trenutku build-a:
		(5.10.50 rpi2+) 33e09977ff3b02967d191801e3361224455acb9d
		(5.10.75 rpi2+) 8db7ef815b849a8a7cdd8e4cd14f41a07b0b0288
		(5.10.95 rpi2+) 6159b7c1195b41aa43b0d2f322e192d84845e622

u-boot:
	2021.07

preuzimanje najnovije verzije buildroot-a
	git clone git://git.buildroot.net/buildroot 
	cd buildroot/
<Preuzeti konkretnu verziju, najbolje rešenje ukoliko se u koraku konfiguracija koristi priložena .config datoteka>
	branch:
		git checkout 2021.08.x
	u trenutku build-a:
		git checkout 4eea95a98980373633c603520b3966b40345171f
	
konfiguracija
	opcija 1)
		opcija a) ručno:
			make raspberrypi2_defconfig
			make xconfig
			...
		opcija b) kopirati priloženu datoteku kao .config u buildroot/
			modules/data/buildroot-2021.08-all.config

		<Uključivanje u-boot.bin u sdcard.img i dodatne datoteke>
			post-build.sh
			genimage-raspberrypi2.cfg
			uboot.env
	opcija 2)
		koristiti patch bootloader/rpi-2-b/src/0001-RPI2-customization-for-EMBEDDED-LINUX-course.patch
		ako je potrebno generisati uboot.env od tekstualnog uboot_env.txt (prethodno je potrebno preuzimanje i bildovanje u-boot-a, vidi gore):
			../../../u-boot/tools/mkenvimage -s 0x4000 -p 0x00 -o uboot.env uboot_env.txt
		git apply <patch>
		make raspberrypi2_defconfig

bildovanje
	make

Osvezavanje dobijenih datoteka na linux-kernel-labs repozitorijumu:
	1) README.txt
		osvezavanje podataka u ovoj datoteci
	2) patch
		dodati sve datoteke koje su izmenjene (git add zeljene datoteke, ostale datoteke najbolje ukloniti da ne bismo imali dirty konfiguraciju. i make config i make, tj. build, najbolje je raditi nakon commita koji sledi da ne bi bio bildovan sa dirty konfiguracijom)
		napraviti novi commit (git commit -m "RPI2 customization for EMBEDDED LINUX course")
		napraviti novi patch (git format-patch HEAD~1)
		prekopirati novi patch na putanju bootloader/rpi-2-b/src/0001-RPI2-customization-for-EMBEDDED-LINUX-course.patch
	3) .config
		prekopirati .config na putanju modules/data/ pod imenom u skladu sa verzijom, npr. cp .config ../../modules/data/buildroot-2021.08-all.config
	4) images (po zelji, ukoliko je promenjena neka od datoteka ispod) 
		bcm2709-rpi-2-b.dtb
			kopirati u ~/linux-kernel-labs/bootloader/rpi-2-b
		boot.vfat
			kopirati u ~/linux-kernel-labs/bootloader/rpi-2-b
		rootfs.ext2
			zapakovati u ~/linux-kernel-labs/bootloader/rpi-2-b/rootfs.ext2.tar.bz2
			tar cvfj ~/linux-kernel-labs/bootloader/rpi-2-b/rootfs.ext2.tar.bz2 rootfs.ext2
		rpi-firmware/
			kopirati u ~/linux-kernel-labs/bootloader/rpi-2-b
		sdcard.img
			zapakovati u ~/linux-kernel-labs/bootloader/rpi-2-b/sdcard.img.tar.bz2
			tar cvfj ~/linux-kernel-labs/bootloader/rpi-2-b/sdcard.img.tar.bz2 sdcard.img
		u-boot.bin
			kopirati u ~/linux-kernel-labs/bootloader/rpi-2-b
		uboot.env*
			kopirati u ~/linux-kernel-labs/bootloader/rpi-2-b
		zImage
			kopirati u ~/linux-kernel-labs/bootloader/rpi-2-b
	5) RFS
		mountovati rootfs.ext2 na neki direktorijum, npr. sudo mount rootfs.ext2 ~/linux-kernel-labs/modules/nfsroot/
		zapakovati sve datoteke iz mountovanog RFS u ~/linux-kernel-labs/modules/rootfs.tar.bz2, npr. sudo tar cvfj ../rootfs.tar.bz2 ./*

=========================================================================
koriscenje usb2serial za konzolu
=========================================================================
1) u kernelu ukljciti staticki
    CONFIG_USB_SERIAL=y
    CONFIG_USB_SERIAL_CONSOLE=y
    CONFIG_USB_SERIAL_PL2303=y
2) u /etc/inittab dodati (ili zameniti postojeci ttyAMA0)
    /dev/ttyUSB0::respawn:/sbin/getty -L  /dev/ttyUSB0 115200 vt100 # USB_SERIAL
3) u bootargs
    console = ttyUSB0,115200
