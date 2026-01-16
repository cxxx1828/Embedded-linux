// SPDX-License-Identifier: GPL-2.0-only

#include <linux/init.h>

#include <linux/module.h>

#include <linux/slab.h>

#include <linux/gfp.h>

#include <linux/kernel.h>

#include <linux/moduleparam.h>

#include <linux/printk.h>

#include <linux/fs.h>

#include <linux/device.h>

#include <linux/cdev.h>

#include <linux/uaccess.h>

#include <linux/string.h>

#define IME_UREDJAJA "zadatak1"

MODULE_LICENSE("GPL");

static int velicina_bafera = 10;

module_param(velicina_bafera, int, 0600);

MODULE_PARM_DESC(velicina_bafera, "Velicina bafera za modul");

static char *bafer;

static struct cdev moj_cdev;

static dev_t broj_uredjaja;

static int glavni_broj;

static struct class *klasa_uredjaja;

static unsigned int brojac_malih_slova;

static const char *poruka_manje = "Number of lowercase letters is less than 4.\n";

static const char *poruka_vise  = "Number of lowercase letters is 4 or greater than 4.\n";

static ssize_t citanje_uredjaja(struct file *fajl, char __user *korisnicki_bafer,

				size_t duzina, loff_t *pomeraj)

{

	const char *poruka = (brojac_malih_slova < 4) ? poruka_manje : poruka_vise;

	size_t duzina_poruke = strlen(poruka);

	if (*pomeraj >= duzina_poruke)

		return 0;

	if (copy_to_user(korisnicki_bafer, poruka, duzina_poruke))

		return -EFAULT;

	*pomeraj += duzina_poruke;

	return duzina_poruke;

}

static ssize_t pisanje_uredjaja(struct file *fajl, const char __user *korisnicki_bafer,

				size_t duzina, loff_t *pomeraj)

{

	size_t velicina_upisa = min(velicina_bafera - 1, duzina);

	size_t nekopirano;

	int i;

	nekopirano = copy_from_user(bafer, korisnicki_bafer, velicina_upisa);

	if (nekopirano)

		return -EFAULT;

	bafer[velicina_upisa] = '\0';	/* safe even if velicina_upisa == 0 */

	*pomeraj += velicina_upisa;

	brojac_malih_slova = 0;

	for (i = 0; i < velicina_upisa; i++) {

		if (bafer[i] >= 'a' && bafer[i] <= 'z')

			brojac_malih_slova++;

	}

	return velicina_upisa;

}

static const struct file_operations operacije = {

	.owner	= THIS_MODULE,

	.read	= citanje_uredjaja,

	.write	= pisanje_uredjaja,

};

static int __init pokretanje_modula(void)

{

	int ret;

	bafer = kmalloc(velicina_bafera, GFP_KERNEL);

	if (!bafer)

		return -ENOMEM;

	ret = alloc_chrdev_region(&broj_uredjaja, 0, 1, IME_UREDJAJA);

	if (ret) {

		pr_err("Neuspela alokacija regiona za uredjaj\n");

		kfree(bafer);

		return ret;

	}

	glavni_broj = MAJOR(broj_uredjaja);

	cdev_init(&moj_cdev, &operacije);

	ret = cdev_add(&moj_cdev, MKDEV(glavni_broj, 0), 1);

	if (ret) {

		pr_err("Neuspelo dodavanje uredjaja\n");

		unregister_chrdev_region(broj_uredjaja, 1);

		kfree(bafer);

		return ret;

	}

	klasa_uredjaja = class_create(THIS_MODULE, IME_UREDJAJA);

	if (IS_ERR(klasa_uredjaja)) {

		ret = PTR_ERR(klasa_uredjaja);

		pr_err("Neuspelo kreiranje klase\n");

		goto err_cdev;

	}

	device_create(klasa_uredjaja, NULL, MKDEV(glavni_broj, 0), NULL, IME_UREDJAJA);

	pr_info("Modul %s uspesno ucitan\n", IME_UREDJAJA);

	return 0;

err_cdev:

	cdev_del(&moj_cdev);

	unregister_chrdev_region(broj_uredjaja, 1);

	kfree(bafer);

	return ret;

}

static void __exit zaustavljanje_modula(void)

{

	device_destroy(klasa_uredjaja, MKDEV(glavni_broj, 0));

	class_destroy(klasa_uredjaja);

	cdev_del(&moj_cdev);

	unregister_chrdev_region(broj_uredjaja, 1);

	kfree(bafer);

	pr_info("Modul %s iskljucen\n", IME_UREDJAJA);

}

module_init(pokretanje_modula);

module_exit(zaustavljanje_modula);