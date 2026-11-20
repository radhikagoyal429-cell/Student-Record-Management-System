FROM gcc:latest
WORKDIR /app
COPY studentrecord2.c .
RUN gcc -Wall -o student studentrecord2.c
CMD ["./student"]