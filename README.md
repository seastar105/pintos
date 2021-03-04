# pintos

## How to run pintos 
---
- Docker and root privilege(sudo) needed
- First, clone this repository, then run start_container.sh. 
- Ready

## How to run gdb
---
- First run pintos in container with --gdb option ex) pintos --gdb --qemu -- -q run alarm-multiple
- connect to pintos_container ex) sudo docker exec -it pintos_container bash
- change directory where kernel.o exists. For example, if you are working with Project 1(Threads) cd /pintos/src/threads/build
- run pintos-gdb with kernel ex) pintos-gdb kernel.o
- type debugpintos(or target remote localhost:1234) in gdb, after that time continue(=c). now pintos are running with gdb.

## How to run gdbgui
---
- Run pintos in continaer with --gdb option.
- connect to pintos_container then change directory where kernel exists
- run pintos-gdbgui. ex) pintos-gdbgui kernel.o
- If you are running pintos_conatiner in your local machine, turn on browser and connect to localhost:5000, once kernel terminates you need to refresh browser to run again.
- If you are running pintos_container in remote machine, Change start_container.sh as below
- sudo docker run --rm -it --name pintos_container -v "$(pwd)/src/:/pintos/src" -p __\<Port your remote machine opens\>__:5000 seastar105/pintos-dev  bash
- run pintos-gdbgui kernel.o in container, then connect to __\<Remote Machine IP\>:port__ in browser.
