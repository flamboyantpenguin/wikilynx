FROM fedora:41


RUN dnf update -y
RUN dnf install qt6-qtwebengine-devel cmake wget mesa-libGL-devel rpmdevtools rpmlint -y

ADD SPECS /root/SPECS
ADD build /root

WORKDIR /root

VOLUME /opt/output

CMD ["bash", "./build"]
