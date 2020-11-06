FROM centos:8

RUN curl https://raw.githubusercontent.com/dominique120/vitanza-service/main/build.sh
RUN ./build.sh

EXPOSE 8123
ENTRYPOINT ["~/vts/vts"]