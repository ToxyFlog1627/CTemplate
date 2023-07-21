FROM gcc:13-bookworm
RUN apt update && apt install -y cppcheck
WORKDIR /app
ENTRYPOINT ["./entry.sh"]