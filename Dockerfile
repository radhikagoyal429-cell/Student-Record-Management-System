FROM gcc:latest
WORKDIR /app
COPY newcode.c .
RUN gcc newcode.c -o newcode -lm
CMD ["./newcode"]