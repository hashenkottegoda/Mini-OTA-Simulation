FROM gcc:latest

WORKDIR /app
COPY main.cpp ./
RUN g++ main.cpp -o ecu

CMD ["./ecu"]
