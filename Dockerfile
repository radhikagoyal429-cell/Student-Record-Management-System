FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends gcc libc6-dev ttyd && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY newcode.c .
RUN gcc -Wall newcode.c -o newcode -lm

CMD ["sh", "-c", "ttyd -W -p ${PORT:-7681} ./newcode"]
