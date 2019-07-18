#!/bin/sh
# By Koolshare

ACTION=$1

export PATH=$PATH:/jffs/softcenter/bin:/jffs/softcenter/scripts
export LD_LIBRARY_PATH=/jffs/softcenter/lib:${LD_LIBRARY_PATH}

#logger "Leaving ${0##*/}."
