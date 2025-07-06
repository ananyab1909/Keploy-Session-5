FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libssl-dev \
    lcov \
    wget \
    libjsoncpp-dev \
    uuid-dev \
    zlib1g-dev \
    libbrotli-dev \
    libsqlite3-dev \
    libhiredis-dev \
    postgresql-server-dev-all \
    libmysqlclient-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

RUN curl -fsSL https://ollama.com/install.sh | sh

RUN ollama pull qwen2:0.5b


COPY . /app

WORKDIR /app


CMD ["python3", "built.py"]
