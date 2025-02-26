# growlight by nick black (nickblack@linux.com)

[![Build Status](https://drone.dsscaw.com:4443/api/badges/dankamongmen/growlight/status.svg)](https://drone.dsscaw.com:4443/dankamongmen/growlight)

Block device manager and system installation tool.

https://nick-black.com/dankwiki/index.php/Growlight

Dependencies:

 - libudev 175+
 - libblkid 2.20.1+
 - libpci 3.1.9+
 - libpciaccess 0.13.1+
 - libdevmapper 1.02.74+
 - libatasmart 0.19+
 - libcryptsetup 2.0.2+
 - OpenSSL 1.0.1+
 - mkswap(8) from util-linux
 - badblocks(8), mkfs.ext4(8), mkfs.ext3(8), mkfs.ext2(8) from e2fsprogs

Kernel options:

 - CONFIG_DM_CRYPT (for device mapper encrypt aka LUKS)
 - CONFIG_MD_RAID* (for MDRAID)
 - CONFIG_MSDOS_PARTITION (for msdos partition tables)
 - CONFIG_EFI_PARTITION (for GPT partition tables)
 ... almost certainly more

Build-only dependencies:

 - xsltproc (tested with 20800/10126/815)
 - docbook-xsl (tested with 4.5)
 - autotools (tested with 1.11)
 - pkg-config (tested with 0.29)
 - cunit (tested with 2.1)
 - autoconf-archive (tested with 2018+)

Building from a git checkout:

 - autoreconf -sfi -Wall,error
 - continue from "Building from a tarball"

Building from a tarball:

 - ./configure && make check && sudo make install

## Using it

See the man page, or press 'H' in curses mode, or type "help" in readline mode.
