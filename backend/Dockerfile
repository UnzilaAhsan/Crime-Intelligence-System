FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libmysqlclient-dev \
    libjsoncpp-dev \
    mysql-client \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Create app directory
WORKDIR /app

# Copy source code
COPY . .

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Expose port
EXPOSE 8080

# Run the application
CMD ["./build/backend"]
