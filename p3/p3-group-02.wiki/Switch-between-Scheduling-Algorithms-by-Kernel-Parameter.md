# Switch between Scheduling Algorithms by Kernel Parameter
In `/etc/boot.cfg.default`, add three new entries for boot menu:
```
menu=Start latest MINIX 3 default:load_mods /boot/minix_latest/mod*;multiboot /boot/minix_latest/kernel rootdevname=$rootdevname sched_t=0 $args
menu=Start latest MINIX 3 lottery:load_mods /boot/minix_latest/mod*;multiboot /boot/minix_latest/kernel rootdevname=$rootdevname sched_t=1 $args
menu=Start latest MINIX 3 edf:load_mods /boot/minix_latest/mod*;multiboot /boot/minix_latest/kernel rootdevname=$rootdevname sched_t=2 $args
```
In `/kernel/main.c`, obtain the value of `sched_t` from environment:
```
value = env_get("sched_t");
if(value)
    sched_t = atoi(value);
if(!value || sched_t < 0 || sched_t > 2)
    sched_t = 0;
```
In `/commands/update_bootcfg/update_bootcfg.sh`, move the location of `DEFAULTCFG` from `DEFAULTCFG=/etc/boot.cfg.default` to `DEFAULTCFG=/usr/src/etc/boot.cfg.default`.

![mini3_booting](https://github.com/yihuajack/pics-for-uploading/raw/master/img/minix3_booting.png)