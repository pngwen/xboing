#!/bin/sh -
# XBoing version control script. Based on one found in xtank distribution.
# last modified: 6th August 1993
#
# =========================================================================
# 
# $Id: version.sh,v 1.1.1.1 1994/12/16 01:36:45 jck Exp $
# $Source: /usr5/legends/jck/xb/master/xboing/version.sh,v $
# $Revision: 1.1.1.1 $
# $Date: 1994/12/16 01:36:45 $
# 
# $Log: version.sh,v $
# Revision 1.1.1.1  1994/12/16  01:36:45  jck
# The XBoing distribution requires configuration management. This is why the
# cvs utility is being used. This is the initial import of all source etc..
#
# 
# =========================================================================
#

if [ ! -r .version ]; then
	echo 0 > .version
fi

touch .version
v=`cat .version` u=${USER-root} d=`pwd` h=`hostname` t=`date` arch=`uname -a`

FILE=version.c

echo "#include \"copyright.h\"" > ${FILE}
echo "char *dateString = \"${t}\";" > ${FILE}
echo "char *whoString = \"${u}\";" >> ${FILE}
echo "char *machineString = \"${arch}\";" >> ${FILE}
echo "int buildNum = ${v};" >> ${FILE}

cycle=`cat .version`
cycle=`expr $cycle + 1`

echo $cycle > .version
