#!/bin/bash

BLUE='\033[0;34m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RED='\033[0;31m'
CYAN='\033[0;36m'
PLAIN='\033[0m'
BOLD='\033[1m'
DIM='\033[2m'

echo -e "${BLUE}     ____    ${GREEN}${BOLD}SHE#GANG${PLAIN}"
echo -e "${BLUE}  __/ / /_   ${PLAIN}${DIM}bash script for build from sources with make${PLAIN}"
echo -e "${BLUE} /_  . __/   ${PLAIN}${DIM}Powerful command interpreter (shell) for linux written in C${PLAIN}"
echo -e "${BLUE}/_    __/    ${CYAN}${DIM}maintained by alxvdev${PLAIN}"
echo -e "${BLUE} /_/_/       ${CYAN}https://github.com/alxvdev/shegang${PLAIN}"
echo -e "${PLAIN}"

if ! [ -f /etc/lsb-release ] || ! grep -q "DISTRIB_ID=Ubuntu" /etc/lsb-release; then
	echo -e "${RED}This script is designed for ${BOLD}Ubuntu Linux. Exiting.${PLAIN}"
	exit 1
fi

if [[ $EUID -ne 0 ]]; then
	echo -e "${RED}This script requires superuser privileges. Run it with 'sudo'.${PLAIN}"
	exit 1
fi

echo -e "${BLUE}Installing core dependencies...${PLAIN}"
apt install -y -qq  \
	readline-devel \
	clang \
	git \
	base-devel \
	make \
	python-pip \
	python3 > /dev/null 2>&1

if [[ $? -eq 0 ]]; then
	echo -e "${GREEN}Core dependencies installed successfully.${PLAIN}"
else
	echo -e "${RED}Error installing core dependencies.${PLAIN}"
	exit 1
fi

echo -e "${BLUE}Building...${PLAIN}"
cd .. && make build

echo -e "${BLUE}Installing...${PLAIN}"
cd .. && make install

echo -e "${BLUE}Cleaning...${PLAIN}"
cd .. && make clean

echo -e "${GREEN}All required dependencies have been installed and all source codes is compiled!${PLAIN}"
