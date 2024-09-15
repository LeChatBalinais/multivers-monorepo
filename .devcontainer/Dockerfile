FROM mcr.microsoft.com/devcontainers/base:debian

RUN sudo apt-get update && sudo apt-get upgrade -y

RUN sudo apt update && sudo apt install software-properties-common -y &&\
    wget https://apt.llvm.org/llvm.sh && chmod +x ./llvm.sh && sudo  ./llvm.sh 16 all && sudo rm ./llvm.sh

RUN sudo apt-get install python3 python3-pip pipx -y && pipx ensurepath

RUN sudo wget -O /usr/local/bin/buildifier https://github.com/bazelbuild/buildtools/releases/download/v6.1.2/buildifier-windows-amd64.exe &&\
    sudo chmod 755 /usr/local/bin/buildifier

RUN pip install conan --break-system-packages &&\
    pip install cmake --break-system-packages
