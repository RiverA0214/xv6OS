# CAMBIO IMPORTANTE: Usamos Ubuntu 24.04 que trae QEMU > 7.2
FROM ubuntu:24.04

# Evitamos preguntas interactivas
ENV DEBIAN_FRONTEND=noninteractive

# Actualizamos e instalamos TODO lo necesario (incluyendo 'bc' y 'curl' por si acaso)
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    gdb-multiarch \
    qemu-system-misc \
    gcc-riscv64-linux-gnu \
    binutils-riscv64-linux-gnu \
    bc \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Directorio de trabajo
WORKDIR /xv6

# Comando por defecto
CMD ["/bin/bash"]
