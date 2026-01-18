# In-Tree kernel module building

Neka je zadatak1 folder u kom se nalazi out-of-tree kernel modul (instalira se koristeći `insmod` komande).

```bash
cp -r hello ~/linux-kernel-labs/src/linux/drivers/
```

zadatak1 folder treba da sadrži Makefile, i Kconfig fajlove, koji će imati sledeći sadržaj (suština je ispratiti sintaksu i formu):

```bash
# Kconfig
# SPDX-License-Identifier: GPL-2.0-only
config ZAD1
	tristate "Zadatak1 kernel module example"
	default m
	help
		This module is created for the purposes of
		OSLuNR laboratory exercises
```

```bash
# Makefile 
obj-$(CONFIG_HELLO) += hello_version.o
```

Primetiti ZAD1 kao konfiguracioni naziv, koji će se kasnije koristiti u okviru `.config`, tj. `make xconfig`.

Ovo nije dovoljno da se modul doda u okviru Linux izvornog repozitorijuma. Takođe je potrebno dodati sledeće u okviru fajla `src/linux/drivers/misc/Kconfig` pri kraju sadržaja fajla:

```bash
# ... 
source "drivers/misc/cardreader/Kconfig"
source "drivers/misc/habanalabs/Kconfig"
source "drivers/misc/uacce/Kconfig"
source "drivers/misc/zadatak1/Kconfig" # Ovu liniju treba dodati
endmenu
```

Nakon ovoga, kad uradimo `make xconfig`, dobijamo novu konfiguracionu opciju ZAD1 (CONFIG_ZAD1), koja se može postaviti na vrednosti Y/N/M (pogledati `tristate` liniju u Kconfig-u za zadatak1).

Kada se kompajlira kernel ponovo, ukoliko je ZAD1 postavljen na Y, kernel modul će biti ugrađen statički. Ukoliko je ZAD1 postavljen na M, tada će zadatak1 biti postavljen kao dinamički modul, koji se može aktivirati preko komande `modprobe zadatak1_version`. U ostalom, zadatak1 neće uopšte biti uključen u kernel. 


git diff HEAD HEAD~1 > linux_nina_dragicevic_ra_148_2021.txt
