FROM centos:8
ARG AWS_ACCES_KEY
ARG AWS_SECRET_KEY
ARG AWS_SESSION_TOKEN

# Update and install dependencies
RUN dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-8.noarch.rpm
RUN dnf install -y git boost-devel make cmake3 gcc-c++ mariadb-devel libcurl-devel openssl-devel libuuid-devel pulseaudio-libs-devel nano cryptopp-devel jq && dnf clean all
RUN dnf -y --enablerepo=PowerTools install moreutils && dnf clean all

# Clone, build and install aws-skd-cpp
RUN git clone https://github.com/aws/aws-sdk-cpp.git /usr/vts_work/aws-sdk-cpp && \
	mkdir /usr/vts_work/aws-sdk-cpp/build && cd /usr/vts_work/aws-sdk-cpp/build && \
	cmake3 -DBUILD_ONLY="dynamodb" -DENABLE_TESTING=OFF -DCUSTOM_MEMORY_MANAGEMENT=OFF -DBUILD_SHARED_LIBS=OFF /usr/vts_work/aws-sdk-cpp && \
    make &&  make install && \
	rm -rf /usr/vts_work/aws-sdk-cpp
	
# Clone, build and install served
RUN git clone https://github.com/meltwater/served.git /usr/vts_work/served && \
	mkdir /usr/vts_work/served/build && cd /usr/vts_work/served/build && \
	cmake3 -DSERVED_BUILD_EXAMPLES=OFF -DSERVED_BUILD_TESTS=OFF -DSERVED_BUILD_SHARED=ON /usr/vts_work/served && \
	make &&  make install && rm -rf /usr/vts_work/served
RUN ln /usr/local/lib/libserved.so.1.4 /usr/lib64/libserved.so.1.4

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
	cmake3 -DDB_DYNAMO=ON /usr/vts_work/vitanza-service && \
	make && \
	mv /usr/vts_work/vitanza-service/build/vts /bin_vitanza/vts && \
	cp /usr/vts_work/vitanza-service/config.json /bin_vitanza/config.json && \
	rm -rf /usr/vts_work/vitanza-service

EXPOSE 8123 80
ENTRYPOINT ["/bin_vitanza/vts"]