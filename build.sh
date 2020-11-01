#!/bin/bash
sudo dnf update -y && sudo dnf upgrade -y
sudo dnf install -y git boost-devel make cmake3 gcc-c++ mariadb-devel libcurl-devel openssl-devel libuuid-devel pulseaudio-libs-devel nano cryptopp-devel


sudo /bin/dd if=/dev/zero of=/var/swap.1 bs=1M count=1024
sudo /sbin/mkswap /var/swap.1
sudo chmod 600 /var/swap.1
sudo /sbin/swapon /var/swap.1


git clone https://github.com/aws/aws-sdk-cpp.git
mkdir aws-sdk-cpp.build && cd aws-sdk-cpp.build
cmake3 -DBUILD_ONLY="dynamodb" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF  -DBUILD_SHARED_LIBS=OFF ../aws-sdk-cpp
sudo make && sudo make install

cd ..

git clone https://github.com/meltwater/served.git
mkdir served.build && cd served.build
cmake3 -DSERVED_BUILD_EXAMPLES=OFF -DSERVED_BUILD_TESTS=OFF -DSERVED_BUILD_SHARED=ON -DSERVED_BUILD_STATIC=ON ../served
make && sudo make install
sudo ln /usr/local/lib/libserved.so.1.4 /usr/lib64/libserved.so.1.4

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
cmake3 -DDB_DYNAMO=ON ../vitanza-service
make

mv vts ../vts/vts
cd ..
cp vitanza-service/config.json vts/config.json