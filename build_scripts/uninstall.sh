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
echo -e "${BLUE}  __/ / /_   ${PLAIN}${DIM}bash script for remove shegang${PLAIN}"
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

read -p "Are you sure you want to remove shegang? (y/n) > " confirm

confirm=$(echo "$confirm" | tr '[:upper:]' '[:lower:]')

if [[ "$confirm" == "y" || "$confirm" == "yes" || "$confirm" == 'д' || "$confirm" == "да" ]]; then
	echo -e "${YELLOW}Removing shegang...${PLAIN}"
else
	echo -e "${RED}Operation cancelled.${PLAIN}"
	exit 1
fi

echo -e "${BLUE}Updating package lists...${PLAIN}"
apt update -qq > /dev/null 2>&1

echo -e "${BLUE}Removing core dependencies...${PLAIN}"
apt remove --noconfirm  \
	readline-devel \
	clang \
	git \
	base-devel \
	make \
	python-pip \
	python3 > /dev/null 2>&1

if [[ $? -eq 0 ]]; then
	echo -e "${GREEN}Core dependencies removed successfully.${PLAIN}"
else
	echo -e "${RED}Error removing core dependencies.${PLAIN}"
	exit 1
fi

echo -e "${BLUE}Removing binary...${PLAIN}"
if [ $CURRDIR == "build_scripts" ]; then
	cd .. && make remove
	cd build_scripts
elif [ $CURRDIR == "shegang" ]; then
	make remove
	cd build_scripts
fi
echo -e "${GREEN}Binary removed successfully.${PLAIN}"

echo -e "${GREEN}All required dependencies and binary have been removed.${PLAIN}"
