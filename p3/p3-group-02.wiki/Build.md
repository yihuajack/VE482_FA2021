To build kernel only, under `root`
```
cd /usr/src/releasetools
make hdboot
reboot
```
To build all including libraries, drivers, and servers, under `root`
```
cd /usr/src
make build
reboot
```