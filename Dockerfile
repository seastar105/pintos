FROM ubuntu:18.04
LABEL maintainer="seastar105@gmail.com"

# Essential programs to run pintos with qemu
RUN apt-get update && apt-get install -y --no-install-recommends \
	qemu-system-i386 \
	git \
	gcc \
	gdb \
	make \
&&	apt-get clean && rm -rf /var/lib/apt/list/* \
&&	ln -s /usr/bin/qemu-system-i386 /usr/bin/qemu

ENV PINTOSHOME=/pintos
ENV PATH=$PINTOSHOME/src/utils:$PATH
EXPOSE 5000

WORKDIR /pintos
# Now you can execute pintos like pintos --qemu -- -q run alarm-multiple after compile

RUN apt-get update && apt-get install -y \
	ctags \
	cscope \
	vim \
	python3 \
	python3-pip \
	netbase \
	ssh

RUN pip3 install --upgrade gdbgui
