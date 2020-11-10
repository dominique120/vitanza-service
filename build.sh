#!/bin/bash
sudo dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
ARCH=$( /bin/arch )
sudo subscription-manager repos --enable "codeready-builder-for-rhel-8-${ARCH}-rpms"
sudo dnf update -y && sudo dnf upgrade -y
sudo dnf install -y git boost-devel make cmake3 gcc-c++ mariadb-devel libcurl-devel openssl-devel libuuid-devel pulseaudio-libs-devel nano cryptopp-devel snapd


sudo /bin/dd if=/dev/zero of=/var/swap.1 bs=1M count=1024
sudo /sbin/mkswap /var/swap.1
sudo chmod 600 /var/swap.1
sudo /sbin/swapon /var/swap.1


git clone https://github.com/aws/aws-sdk-cpp.git
mkdir aws-sdk-cpp.build && cd aws-sdk-cpp.build
cmake3 -DBUILD_ONLY="dynamodb; s3" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF  -DBUILD_SHARED_LIBS=OFF ../aws-sdk-cpp
sudo make && sudo make install

cd ..


systemctl enable --now snapd.socket
ln -s /var/lib/snapd/snap /snap
snap install cmake --classic

git clone https://github.com/yhirose/cpp-httplib.git
mkdir cpp-httplib.build && cd cpp-httplib.build
/snap/bin/cmake ../cpp-httplib
make && sudo make install

cd ..

git clone https://github.com/nlohmann/json.git
mkdir json.build && cd json.build
cmake3 -DJSON_BuildTests=OFF ../json
make && sudo make install

cd .. 

git clone https://github.com/Thalhammer/jwt-cpp.git
mkdir jwt-cpp.build && cd jwt-cpp.build
cmake3 -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF ../jwt-cpp
make && sudo make install

cd ..

git clone https://github.com/dominique120/vitanza-service.git
mkdir vts && mkdir vts.build && cd vts.build
/snap/bin/cmake -DDB_DYNAMO=ON ../vitanza-service
make

mv vts ../vts/vts
cd ..
cp vitanza-service/config.json vts/config.json
cp vitanza-service/launch.sh vts/launch.sh
