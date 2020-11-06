FROM centos:8

RUN dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf update -y &&  dnf upgrade -y
RUN dnf install -y git boost-devel make cmake3 gcc-c++ mariadb-devel libcurl-devel openssl-devel libuuid-devel pulseaudio-libs-devel nano cryptopp-devel jq


RUN git clone https://github.com/aws/aws-sdk-cpp.git
RUN mkdir aws-sdk-cpp.build && cd aws-sdk-cpp.build
RUN cmake3 -DBUILD_ONLY="dynamodb" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF  -DBUILD_SHARED_LIBS=OFF ../aws-sdk-cpp
RUN  make &&  make install

RUN cd ..


RUN git clone https://github.com/meltwater/served.git
RUN mkdir served.build && cd served.build
RUN cmake3 -DSERVED_BUILD_EXAMPLES=OFF -DSERVED_BUILD_TESTS=OFF -DSERVED_BUILD_SHARED=ON -DSERVED_BUILD_STATIC=ON ../served
RUN make &&  make install
RUN  ln /usr/local/lib/libserved.so.1.4 /usr/lib64/libserved.so.1.4

RUN cd ..

RUN git clone https://github.com/nlohmann/json.git
RUN mkdir json.build && cd json.build
RUN cmake3 -DJSON_BuildTests=OFF ../json
RUN make &&  make install

RUN cd .. 

RUN git clone https://github.com/Thalhammer/jwt-cpp.git
RUN mkdir jwt-cpp.build && cd jwt-cpp.build
RUN cmake3 -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF ../jwt-cpp
RUN make &&  make install

RUN cd ..

RUN git clone https://github.com/dominique120/vitanza-service.git
RUN mkdir vts && mkdir vts.build && cd vts.build
RUN cmake3 -DDB_DYNAMO=ON ../vitanza-service
RUN make

RUN mv vts ../vts/vts
RUN cd ..
RUN cp vitanza-service/config.json vts/config.json

EXPOSE 8123
ENTRYPOINT ["~/vts/vts"]