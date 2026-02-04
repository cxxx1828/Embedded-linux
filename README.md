Compile the kernel (from `src/linux/`):
- Apply patches if needed (e.g., `zadatak1_defconfig.patch.bz2`).
- Configure with `.config` files provided in each branch.
- Build: `make ARCH=arm CROSS_COMPILE=arm-none-eabi- bcmrpi_defconfig` (adjust for your setup).
