apt-get install -y git cmake build-essential libmariadb-dev-compat libboost-system-dev libcurl4-openssl-dev libssl-dev uuid-dev zlib1g-dev libpulse-dev 


git clone https://github.com/aws/aws-sdk-cpp.git
cd aws-sdk-cpp
mkdir build 
cd build
cmake .. -DBUILD_ONLY="dynamodb" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF  -DBUILD_SHARED_LIBS=OFF
make
make install

cd ..
cd ..

git clone https://github.com/meltwater/served.git
cd served 
mkdir build 
cd build
cmake .. -DSERVED_BUILD_EXAMPLES=OFF -DSERVED_BUILD_TESTS=OFF
make
make install