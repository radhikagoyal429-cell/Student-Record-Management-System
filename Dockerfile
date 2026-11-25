FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends gcc libc6-dev curl ca-certificates && \
    curl -L https://github.com/tsl0922/ttyd/releases/download/1.7.7/ttyd.x86_64 -o /usr/local/bin/ttyd && \
    chmod +x /usr/local/bin/ttyd && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY newcode.c .
RUN gcc -Wall newcode.c -o newcode -lm

CMD ["sh", "-c", "ttyd -W -p ${PORT:-7681} ./newcode"]
