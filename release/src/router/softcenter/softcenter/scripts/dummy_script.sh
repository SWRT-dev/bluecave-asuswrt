#!/bin/sh
source /jffs/softcenter/scripts/base.sh

[ -n "$SCAPI" ] && http_response "$1"
