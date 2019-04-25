cmd_/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.o := arm-hisiv300-linux-gcc -Wp,-MD,/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/.hi_vreg.o.d  -nostdinc -isystem /opt/hisi-linux/x86-arm/arm-hisiv300-linux/bin/../lib/gcc/arm-hisiv300-linux-uclibcgnueabi/4.8.3/include -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/include/linux/kconfig.h -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-hi3518ev200/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -marm -fno-dwarf2-cfi-asm -fno-aggressive-loop-optimizations -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mabi=aapcs-linux -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -msoft-float -Uarm -Wframe-larger-than=1024 -fno-stack-protector -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO -Wall -DVER_X=1 -DVER_Y=0 -DVER_Z=0 -DVER_P=0 -DVER_B=10 -D ENABLE_JPEGEDCF -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/code/mkp/include -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/code/arch/hi3518e/include -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/code/mkp_inc -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/vreg -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/vreg/arch/hi3518e -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/include -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/extdrv/i2c -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/extdrv/hi_i2c -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/extdrv/ssp-sony -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/extdrv/i2c -I/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/extdrv/hi_i2c -D ENABLE_JPEGEDCF -DHI_XXXX -DHI_GPIO_XXX -DHI_RELEASE  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(hi_vreg)"  -D"KBUILD_MODNAME=KBUILD_STR(hi3518e_isp)" -c -o /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.o /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.c

source_/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.o := /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.c

deps_/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.o := \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_vreg.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_type.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_comm_isp.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_errno.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_debug.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_common.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_math.h \
  include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/posix_types.h \
  include/asm-generic/posix_types.h \
  include/linux/cache.h \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
  include/linux/sysinfo.h \
  /opt/hisi-linux/x86-arm/arm-hisiv300-linux/lib/gcc/arm-hisiv300-linux-uclibcgnueabi/4.8.3/include/stdarg.h \
  include/linux/linkage.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/bitops.h \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/irqflags.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/hwcap.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/le.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/ext2-atomic-setbit.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/div64.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/compiler.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/debug/bugverbose.h) \
    $(wildcard include/config/arm/lpae.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/arm/thumbee.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
    $(wildcard include/config/cpu/use/domains.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/barrier.h \
    $(wildcard include/config/cpu/32v6k.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arch/has/barriers.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/outercache.h \
    $(wildcard include/config/outer/cache/sync.h) \
    $(wildcard include/config/outer/cache.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/memory.h \
    $(wildcard include/config/need/mach/memory/h.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/have/tcm.h) \
    $(wildcard include/config/arm/patch/phys/virt.h) \
    $(wildcard include/config/phys/offset.h) \
    $(wildcard include/config/hisi/snapshot/boot.h) \
  include/linux/const.h \
  arch/arm/include/generated/asm/sizes.h \
  include/asm-generic/sizes.h \
  arch/arm/mach-hi3518ev200/include/mach/memory.h \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/prove/rcu.h) \
  include/linux/rwlock_types.h \
  include/linux/spinlock_up.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/processor.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/arm/errata/754327.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/hw_breakpoint.h \
  include/linux/rwlock.h \
  include/linux/spinlock_api_up.h \
  include/linux/atomic.h \
    $(wildcard include/config/arch/has/atomic/or.h) \
    $(wildcard include/config/generic/atomic64.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/atomic.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/cmpxchg.h \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
    $(wildcard include/config/cpu/v6.h) \
  include/asm-generic/cmpxchg-local.h \
  include/asm-generic/cmpxchg.h \
  include/asm-generic/atomic-long.h \
  include/asm-generic/atomic64.h \
  include/linux/math64.h \
  include/linux/jiffies.h \
  include/linux/timex.h \
  include/linux/param.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/timex.h \
  arch/arm/mach-hi3518ev200/include/mach/timex.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_defines.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_comm_video.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_isp_debug.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_comm_isp.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_drv_vreg.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/include/hi_comm_3a.h \
  include/linux/fs.h \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  include/linux/limits.h \
  include/linux/ioctl.h \
  arch/arm/include/generated/asm/ioctl.h \
  include/asm-generic/ioctl.h \
  include/linux/blk_types.h \
    $(wildcard include/config/blk/dev/integrity.h) \
  include/linux/wait.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/current.h \
  include/linux/kdev_t.h \
  include/linux/dcache.h \
  include/linux/rculist.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/rcu/torture/test.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/rcu/trace.h) \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tiny/preempt/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/preempt/rt.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/string.h \
  include/linux/bug.h \
  include/linux/completion.h \
  include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  include/linux/rcutiny.h \
  include/linux/rculist_bl.h \
  include/linux/list_bl.h \
  include/linux/bit_spinlock.h \
  include/linux/path.h \
  include/linux/stat.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/stat.h \
  include/linux/radix-tree.h \
  include/linux/prio_tree.h \
  include/linux/pid.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
    $(wildcard include/config/have/arch/mutex/cpu/relax.h) \
  include/linux/mutex-debug.h \
  include/linux/capability.h \
  include/linux/semaphore.h \
  include/linux/fiemap.h \
  include/linux/shrinker.h \
  include/linux/migrate_mode.h \
  include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  include/linux/errno.h \
  arch/arm/include/generated/asm/errno.h \
  include/asm-generic/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/percpu_counter.h \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/pfn.h \
  arch/arm/include/generated/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  include/linux/dqblk_xfs.h \
  include/linux/dqblk_v1.h \
  include/linux/dqblk_v2.h \
  include/linux/dqblk_qtree.h \
  include/linux/nfs_fs_i.h \
  include/linux/fcntl.h \
  /home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y/arch/arm/include/asm/fcntl.h \
  include/asm-generic/fcntl.h \
  include/linux/err.h \

/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.o: $(deps_/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.o)

$(deps_/home/aston/sambashare/Hi3518E_SDK_V1.0.3.0/mpp/component/isp/firmware/drv/../vreg/hi_vreg.o):
