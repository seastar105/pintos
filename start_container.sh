# Start pintos-dev container
sudo docker run --rm -it --name pintos_container -v "$(pwd)/src/:/pintos/src" -p 5000:5000 seastar105/pintos-dev bash
