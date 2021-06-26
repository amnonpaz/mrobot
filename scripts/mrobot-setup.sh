#!/bin/sh

# Setting up variables
SCRIPT=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT`

TMP_DIR=/tmp/mrobot
INSTALL_DIR=/etc/mrobot
TEMPLATE_FILE=${SCRIPT_DIR}/template.conf
CONF_FILE=mrobot.conf
TMP_FILE=${TMP_DIR}/${CONF_FILE}

# Welcome message
echo
echo "+------------------------+"
echo "|   mRobot Agent setup   |"
echo "+------------------------+"
echo

# Reading configuration from user
echo "What is the client name?"
read CLIENT_NAME

echo "What is the MqTT broker's URL?"
read BROKER_URL

echo "What is the MqTT broker's port?"
read BROKER_PORT

# Creating temporary file
mkdir -p ${TMP_DIR}
cp ${TEMPLATE_FILE} ${TMP_FILE} 

if [ "$?" -ne "0" ] ; then
    echo "Error creating temporary files"
    exit 1
fi

# Writing configuration
sed -i "s/<MQTT_CLIENT_NAME>/${CLIENT_NAME}/g" ${TMP_FILE} 
sed -i "s/<MQTT_BROKER_URL>/${BROKER_URL}/g" ${TMP_FILE} 
sed -i "s/<MQTT_BROKER_PORT>/${BROKER_PORT}/g" ${TMP_FILE} 

# Installing file
mkdir -p ${INSTALL_DIR}
cp ${TMP_FILE} ${INSTALL_DIR} 

# Cleaning up temporart directory
rm -rf ${TMP_DIR}

# Done
echo
echo "* Setup done *"
