FROM ubuntu
RUN apt update && \
    apt install -y gcc make cmake wget clang pkg-config git

RUN git clone https://github.com/Microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh && \
    cd /vcpkg/ && \
    ./vcpkg install mosquitto tgbot-cpp

RUN git clone https://github.com/opencv/opencv.git && \
    mkdir -p build && cd build && \
    cmake ../opencv && \
    make -j4 && \
    make install

COPY DeliveryBot /

RUN cd /DeliveryBot/ && mkdir build && cd /DeliveryBot/build && \
    cmake .. && \
    cmake --build .

RUN cd /DeliveryBot/build/ && ./DeliveryBotBinary
