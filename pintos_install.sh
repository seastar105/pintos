# clone up-to-date pintos source codes
git clone git://pintos-os.org/pintos-anon
mkdir $HOME/pintos/
mv pintos-anon/src $HOME/pintos/
rm -rf pintos-anon
echo 'export PATH=$HOME/pintos/src/utils/:$PATH' >> $HOME/.bashrc
source $HOME/.bashrc

# Change emulator to qemu
sed -i 's/bochs/qemu/' $HOME/pintos/src/*/Make.vars
sed -i "s|\/usr\/class\/cs140\/pintos\/pintos\/src|$HOME\/pintos\/src|" $HOME/pintos/src/utils/pintos-gdb

sudo apt-get update && sudo apt-get install -y qemu
sudo ln -s /usr/bin/qemu-system-i386 /usr/bin/qemu

sudo apt-get install -y build-essential

# now you can build program in src/threads but cannot execute with qemu
# Make SIMULATROR in src/threads/Make.vars to qemu
# If you want to debug pintos with gdb, Modify GDBMACROS in src/utils/pintos-gdb to $HOME/pintos/src/misc/gdb-macros
