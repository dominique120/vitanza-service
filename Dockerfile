FROM centos:8

# Update and install dependencies
RUN dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf install -y git boost-devel make cmake3 gcc-c++ mariadb-devel libcurl-devel openssl-devel libuuid-devel pulseaudio-libs-devel cryptopp-devel snapd && dnf clean all
RUN dnf -y --enablerepo=PowerTools install moreutils && dnf clean all

# Clone, build and install aws-skd-cpp
RUN git clone https://github.com/aws/aws-sdk-cpp.git /usr/vts_work/aws-sdk-cpp && \
	mkdir /usr/vts_work/aws-sdk-cpp/build && cd /usr/vts_work/aws-sdk-cpp/build && \
	cmake3 -DBUILD_ONLY="dynamodb s3" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF -DBUILD_SHARED_LIBS=OFF /usr/vts_work/aws-sdk-cpp && \
    make &&  make install && \
	rm -rf /usr/vts_work/aws-sdk-cpp
	
# Enable cmake 3.16+
RUN snap install cmake --classic
	
# Clone, build and install httplib
RUN git clone https://github.com/yhirose/cpp-httplib.git /usr/vts_work/httplib && \
	mkdir /usr/vts_work/httplib/build && cd /usr/vts_work/httplib/build && \
	/snap/bin/cmake /usr/vts_work/cpp-httplib && \
	make &&  make install && rm -rf /usr/vts_work/httplib

# Clone, build and install json
RUN git clone https://github.com/nlohmann/json.git /usr/vts_work/json && \
	mkdir /usr/vts_work/json/build && cd /usr/vts_work/json/build && \
	cmake3 -DJSON_BuildTests=OFF /usr/vts_work/json && \
	make &&  make install && rm -rf /usr/vts_work/json

# Clone, build and install jwt-cpp
RUN git clone https://github.com/Thalhammer/jwt-cpp.git /usr/vts_work/jwt-cpp && \
	mkdir /usr/vts_work/jwt-cpp/build && cd /usr/vts_work/jwt-cpp/build && \
	cmake3 -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF /usr/vts_work/jwt-cpp && \
	make &&  make install && rm -rf /usr/vts_work/jwt-cpp

RUN mkdir /bin_vitanza

# Clone, build and install vitanza
RUN git clone https://github.com/dominique120/vitanza-service.git /usr/vts_work/vitanza-service && \
	mkdir /usr/vts_work/vitanza-service/build && cd /usr/vts_work/vitanza-service/build && \
	/snap/bin/cmake -DDB_DYNAMO=ON /usr/vts_work/vitanza-service && \
	make && \
	mv /usr/vts_work/vitanza-service/build/vts /bin_vitanza/vts && \
	cp /usr/vts_work/vitanza-service/config.json /bin_vitanza/config.json && \
	rm -rf /usr/vts_work/vitanza-service
	
EXPOSE 8123
WORKDIR /bin_vitanza
VOLUME /bin_vitanza
ENTRYPOINT /bin_vitanza/vts 0.0.0.0 80
