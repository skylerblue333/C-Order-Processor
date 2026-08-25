FROM alpine:3.22 AS build
RUN apk add --no-cache build-base
WORKDIR /src
COPY src/main.c ./main.c
RUN mkdir -p /out \
    && gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -O2 -static -s -o /out/sky-order-core main.c

FROM scratch
COPY --from=build /out/sky-order-core /sky-order-core
USER 65532:65532
ENTRYPOINT ["/sky-order-core"]
