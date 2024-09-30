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
echo -e "${BLUE}  __/ / /_   ${PLAIN}${DIM}bash script for update shegang${PLAIN}"
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

echo -e "${YELLOW}Update shegang${PLAIN}"

echo -e "${BLUE}Get updates from repository (pull)...${PLAIN}"

command=$(git pull)

if [[ "$command" = "Already up to date." ]]; then 
	echo -e "${GREEN}All already up to date! Exit...${PLAIN}"
	exit 0
fi

if [[ $? -eq 0 ]]; then
	echo -e "${GREEN}Repository updated successfully.${PLAIN}"
else
	echo -e "${RED}Error updating repository.${PLAIN}"
	exit 1
fi

echo -e "${BLUE}Updating package lists...${PLAIN}"
apt update -qq > /dev/null 2>&1

echo -e "${BLUE}Upgrade packages...${PLAIN}"
apt upgrade -y -qq > /dev/null 2>&1

if [[ $? -eq 0 ]]; then
	echo -e "${GREEN}Packages updated&upgraded successfully.${PLAIN}"
else
	echo -e "${RED}Error updating packages.${PLAIN}"
	exit 1
fi

if [[ $? -eq 0 ]]; then
	echo -e "${GREEN}Additional dependencies updated successfully.${PLAIN}"
else
	echo -e "${RED}Error updating additional dependencies.${PLAIN}"
	exit 1
fi

cho -e "${BLUE}Building...${PLAIN}"
cd .. && make build

echo -e "${BLUE}Installing...${PLAIN}"
cd .. && make install

echo -e "${BLUE}Cleaning...${PLAIN}"
cd .. && make clean

echo -e "${GREEN}All required dependencies have been updated and the shegang rebuilded.${PLAIN}"
