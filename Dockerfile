FROM gcc:13-bookworm
WORKDIR /app
ENTRYPOINT ["./entry.sh"]