FROM ubuntu:24.04 AS build

RUN apt update && apt install -y \
    cmake \
    ninja-build \
    g++ \
    libpcap-dev \
    libboost-program-options-dev

WORKDIR /app

COPY . .

RUN cmake -B build -G Ninja

RUN cmake --build build

CMD ["./build/NetworkTools"]

FROM ubuntu:24.04 

RUN apt update && apt install -y \ 
    libpcap0.8 \
    libboost-program-options1.83.0 \
&& rm -rf /var/lib/apt/lists/*

COPY --from=build /app/build/NetworkTools .

ENTRYPOINT ["./NetworkTools"]

CMD []