FROM centos:8

RUN dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf update -y &&  dnf upgrade -y
RUN dnf install -y git boost-devel make cmake3 gcc-c++ mariadb-devel libcurl-devel openssl-devel libuuid-devel pulseaudio-libs-devel nano cryptopp-devel jq

RUN git clone https://github.com/aws/aws-sdk-cpp.git /usr/vts_work/aws-sdk-cpp
WORKDIR /usr/vts_work/aws-sdk-cpp/build
RUN cmake3 -DBUILD_ONLY="dynamodb" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF -DBUILD_SHARED_LIBS=OFF ..
RUN make &&  make install

RUN git clone https://github.com/meltwater/served.git /usr/vts_work/served
WORKDIR /usr/vts_work/served/build
RUN cmake3 -DSERVED_BUILD_EXAMPLES=OFF -DSERVED_BUILD_TESTS=OFF -DSERVED_BUILD_SHARED=ON -DSERVED_BUILD_STATIC=ON ..
RUN make &&  make install
RUN ln /usr/local/lib/libserved.so.1.4 /usr/lib64/libserved.so.1.4

RUN git clone https://github.com/nlohmann/json.git /usr/vts_work/json
WORKDIR /usr/vts_work/json/build
RUN cmake3 -DJSON_BuildTests=OFF ..
RUN make &&  make install

RUN git clone https://github.com/Thalhammer/jwt-cpp.git /usr/vts_work/jwt-cpp
WORKDIR /usr/vts_work/jwt-cpp/build
RUN cmake3 -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF ..
RUN make &&  make install

RUN git clone https://github.com/dominique120/vitanza-service.git /usr/vts_work/vitanza-service
WORKDIR /usr/vts_work/vitanza-service/build
RUN cmake3 -DDB_DYNAMO=ON ..
RUN make

RUN mkdir /bin_vitanza
RUN mv /usr/vts_work/vitanza-service/build/vts /bin_vitanza/vts
RUN cp /usr/vts_work/vitanza-service/config.json /bin_vitanza/config.json

EXPOSE 8123
ENTRYPOINT ["/bin_vitanza/vts"]