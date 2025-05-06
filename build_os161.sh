#!/bin/bash

#======================================================
# Run this in bash shell
#------------------------------------------------------
#
# Run this function: 
#
# > ./build_os161 3
#
# Here, the script is in test_os161/ and the assignment configuration file is ASST3. 
#
#
#

# Define some colors 
# Source: https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux

red=`tput setaf 1`
green=`tput setaf 2`
yellow=`tput setaf 3`

reset=`tput sgr0`

# Assignment number to concantenate to ASST
assignNo=$1
confPrefix="ASST"
confName="${confPrefix}${assignNo}"


# Full path of directory of the assignment repository
#path2repository=$2

# Go to the student repository. 
# cd $path2repository

#read -p "Press enter to continue"

echo " "
echo "Building kernel-level programs for assignment $confName ..."


#pushd ${PWD}
cd kern/conf

# # Delete output file if one exists
# if test -f $LOGFILE; then
#   echo "File build_output.txt exists. Removing old build_output.txt"
#   rm $LOGFILE
# fi

#./config $confName >>$LOGFILE 2>&1
./config $confName
if [ $? -eq 0 ]; then
    echo "${green}Config succeeded${reset}"
else
    echo "${red}Config failed${reset}"
    exit 1
fi

#read -p "Press enter to continue"


cd ../compile/${confName}

bmake depend
#bmake depend >>$LOGFILE 2>&1
if [ $? -eq 0 ]; then
    echo "${green}bmake depend succeeded${reset}"
else
    echo "${red}bmake depend failed${reset}"
    exit 1
fi

#read -p "Press enter to continue"

#bmake >>$LOGFILE 2>&1
bmake

if [ $? -eq 0 ]; then
    echo "${green}bmake succeeded${reset}"
else
    echo "${red}bmake failed${reset}"
    exit 1
fi

#read -p "Press enter to continue"


#bmake install >>$LOGFILE 2>&1
bmake install

if [ $? -eq 0 ]; then
    echo "${green}bmake install succeeded${reset}"
else
    echo "${red}bmake install failed${reset}"
    exit 1
fi

#read -p "Press enter to continue"

echo " "
echo "Building userland for assignment $confName ..."

# popd

cd ../../../


#bmake includes >>$LOGFILE 2>&1
bmake includes 
if [ $? -eq 0 ]; then
    echo "${green}bmake includes succeeded${reset}"
else
    echo "${red}bmake includes failed${reset}"
    exit 1
fi


#bmake depend >>$LOGFILE 2>&1
bmake depend 
if [ $? -eq 0 ]; then
    echo "${green}bmake depend succeeded${reset}"
else
    echo "${red}bmake depend failed${reset}"
    exit 1
fi

#bmake >>$LOGFILE 2>&1
bmake 
if [ $? -eq 0 ]; then
    echo "${green}bmake succeeded${reset}"
else
    echo "${red}bmake failed${reset}"
    exit 1
fi

#bmake install >>$LOGFILE 2>&1
bmake install
if [ $? -eq 0 ]; then
    echo "${green}bmake install succeeded${reset}"
else
    echo "${red}bmake install failed${reset}"
    exit 1
fi

echo " "
echo "Building done."
echo " "
echo "Now, run ${yellow}sys161 kernel${reset} from inside ${yellow}~/os161/root/${reset}"
echo " "

