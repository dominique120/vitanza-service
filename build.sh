apt-get install -y git cmake build-essential libmariadb-dev-compat libboost-system-dev libcurl4-openssl-dev libssl-dev uuid-dev zlib1g-dev libpulse-dev 



git clone https://github.com/aws/aws-sdk-cpp.git
mkdir aws-sdk-cpp.build && cd aws-sdk-cpp.build
cmake ../aws-cpp-sdk -DBUILD_ONLY="dynamodb" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF  -DBUILD_SHARED_LIBS=OFF
make && make install


git clone https://github.com/meltwater/served.git
mkdir served.build && cd served.build
cmake ../served -DSERVED_BUILD_EXAMPLES=OFF -DSERVED_BUILD_TESTS=OFF -DSERVED_BUILD_SHARED=ON -DSERVED_BUILD_STATIC=ON
make && make install
ln /usr/local/lib/libserved.so.1.4 /usr/lib64/libserved.so.1.4



clone Vitanza Service
mkdir vts.build && cd vts.build
cmake ../vts
make

