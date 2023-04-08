FROM ubuntu
RUN apt update && \
    apt install -y gcc make cmake wget clang pkg-config git curl zip unzip tar

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh && \
    cd /vcpkg/ && \
    ./vcpkg install mosquitto tgbot-cpp

RUN git clone https://github.com/opencv/opencv.git && \
    mkdir -p build && cd build && \
    cmake ../opencv && \
    make -j4 && \
    make install

#COPY DeliveryBot /

#RUN  while :; do cd /app && ls; sleep 1; done
RUN apt-get -y install mosquitto libmosquitto-dev libssl-dev libboost-all-dev libssl-dev libcurl4-openssl-dev


#RUN cd /vcpkg/ && \
#        ./vcpkg install boost


RUN mkdir /app

COPY DeliveryBot/ /app

#RUN while :; do cd /app/build/ && ls && sleep 5; sleep 1; done

RUN cd /app/ && mkdir build && cd /app/build && \
    cmake .. && \
    cmake --build .

RUN cd /app/build/ && ./DeliveryBotBinary
