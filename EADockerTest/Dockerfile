FROM ubuntu

# Update aptitude with new repo
RUN apt-get update

ARG DEBIAN_FRONTEND=noninteractive
# Install software 
RUN apt-get install -y xorg
RUN apt-get install -y xorg-dev
RUN apt-get install -y git make g++ 

RUN git clone --recursive https://github.com/AraujoVE/OSRobotsGame

RUN apt-get install -y libboost-all-dev
RUN apt-get install -y libopenblas-dev liblapack-dev

RUN apt-get install -y wget cmake

RUN wget https://ufpr.dl.sourceforge.net/project/arma/armadillo-10.2.1.tar.xz
RUN tar -xvf armadillo-10.2.1.tar.xz

WORKDIR "/armadillo-10.2.1"

RUN ./configure
RUN make
RUN make install

RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:kisak/kisak-mesa && apt-get update
RUN apt-get install mesa-utils

RUN apt-get install -y ocl-icd-opencl-dev

WORKDIR "/OSRobotsGame"
CMD ["bash", "./dev.bash"]