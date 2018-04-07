#include <linux/magic.h>
#include <linux/root_dev.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#define ROOTFS_SPLIT_NAME "rootfs_data"

#ifdef CONFIG_MTD_SPLIT_FIRMWARE_NAME
#define SPLIT_FIRMWARE_NAME	CONFIG_MTD_SPLIT_FIRMWARE_NAME
#else
#define SPLIT_FIRMWARE_NAME	"unused"
#endif

struct squashfs_super_block {
	__le32 s_magic;
	__le32 pad0[9];
	__le64 bytes_used;
};

static void split_brnimage_kernel(struct mtd_info *master, const char *name,
                                       int offset, int size)
{
	unsigned long buf[4];
	// Assume at most 2MB of kernel image
	unsigned long end = offset + (2 << 20);
	unsigned long part_size = offset + 0x400 - 12;
	size_t len;
	int ret;

	if (strcmp(name, "rootfs") != 0)
		return;
	while (part_size < end) {
		long size_min = part_size - 0x400 - 12 - offset;
		long size_max = part_size + 12 - offset;
		ret = mtd_read(master, part_size, 16, &len, (void *)buf);
		if (ret || len != 16)
			return;

		if (le32_to_cpu(buf[0]) < size_min ||
				le32_to_cpu(buf[0]) > size_max) {
			part_size += 0x400;
			continue;
		}

		if (le32_to_cpu(buf[3]) == SQUASHFS_MAGIC) {
			part_size += 12 - offset;
			__mtd_add_partition(master, "rootfs", offset + part_size,
			                            size - part_size, false);
			return;
		}
		part_size += 0x400;
	}
}

static void split_eva_kernel(struct mtd_info *master, const char *name,
				int offset, int size)
{
#define EVA_MAGIC   0xfeed1281
	unsigned long magic = 0;
	unsigned long part_size = 0;
	size_t len;
	int ret;

	if (strcmp(name, SPLIT_FIRMWARE_NAME) != 0)
		return;

	ret = mtd_read(master, offset, 4, &len, (void *)&magic);
	if (ret || len != sizeof(magic))
		return;

	if (le32_to_cpu(magic) != EVA_MAGIC)
		return;

	ret = mtd_read(master, offset + 4, 4, &len, (void *)&part_size);
	if (ret || len != sizeof(part_size))
		return;

	part_size = le32_to_cpu(part_size) + 0x18;
	part_size = mtd_pad_erasesize(master, offset, len);
	if (part_size + master->erasesize > size)
		return;

	__mtd_add_partition(master, "rootfs", offset + part_size,
			    size - part_size, false);
}

static void split_tplink_kernel(struct mtd_info *master, const char *name,
				int offset, int size)
{
#define TPLINK_MAGIC   0x00000002
	unsigned long magic = 0;
	unsigned long part_size = 0;
	size_t len;
	int ret;

	if (strcmp(name, SPLIT_FIRMWARE_NAME) != 0)
		return;

	ret = mtd_read(master, offset, 4, &len, (void *)&magic);
	if (ret || len != sizeof(magic))
		return;

	if (le32_to_cpu(magic) != TPLINK_MAGIC)
		return;

	ret = mtd_read(master, offset + 0x78, 4, &len, (void *)&part_size);
	if (ret || len != sizeof(part_size))
		return;

	part_size = be32_to_cpu(part_size) + 0x200;
	if (part_size + master->erasesize > size)
		return;

	__mtd_add_partition(master, "rootfs", offset + part_size,
			    size - part_size, false);
}

static void split_squashfs(struct mtd_info *master, const char *name,
				int offset, int size)
{
	struct squashfs_super_block sb;
	int len, ret;

	offset += 0x100;
	size -= 0x100;

	ret = mtd_read(master, offset, sizeof(sb), &len, (void *) &sb);
	if (ret || (len != sizeof(sb)))
		return;

	if (SQUASHFS_MAGIC != le32_to_cpu(sb.s_magic) )
		return;

	if (le64_to_cpu((sb.bytes_used)) <= 0)
		printk(KERN_ALERT "split_squashfs: squashfs is empty in \"%s\"\n",
			master->name);
		return;

	len = (u32) le64_to_cpu(sb.bytes_used);
	len = mtd_pad_erasesize(master, offset, len);
	offset += len;
	size -= len ;
	printk(KERN_INFO "mtd: partition \"%s\" created automatically, ofs=0x%x, len=0x%x\n",
		ROOTFS_SPLIT_NAME, offset, size);

	__mtd_add_partition(master, ROOTFS_SPLIT_NAME, offset,
			    size, false);
}

void arch_split_mtd_part(struct mtd_info *master, const char *name,
				int offset, int size)
{
	split_tplink_kernel(master, name, offset, size);
	split_eva_kernel(master, name, offset, size);
	split_brnimage_kernel(master, name, offset, size);
	split_squashfs(master, name, offset, size);
}
