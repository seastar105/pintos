FROM ubuntu:18.04
LABEL maintainer="seastar105@gmail.com"

# Essential programs to run pintos with qemu
WORKDIR /tmp
RUN sudo apt-get update && sudo apt-get install -y --no-install-recommends \
	git \
	build-essential \
	qemu \
&&	sudo apt-get clean && rm -rf /var/lib/apt/list/* \
&&	git clone https://github.com/seastar105/pintos \
&&	mkdir /pintos && mv ./src /pintos/
&&	rm -rf *

ENV PINTOSHOME=/pintos
ENV PATH=$PINTOSHOME/src/utils:$PATH

WORKDIR /pintos
