/*##################################################################################################
# "Copyright (c) 2013 Intel Corporation                                                            #
# DISTRIBUTABLE AS SAMPLE SOURCE SOFTWARE                                                          #
# This Distributable As Sample Source Software is subject to the terms and conditions              #
# of the Intel Software License Agreement for the Intel(R) Cable and GW Software Development Kit"  #
##################################################################################################*/

#ifndef __FAPI_WLAN_BAND_STEERING_H_
#define __FAPI_WLAN_BAND_STEERING_H_

#include "fapi_wlan_private.h"


/* API Functions */
void fapiBandSteeringAppStart(int signalStrengthThreshold_2_4, int signalStrengthThreshold_5, int intervalInSeconds, int toleranceInSeconds, int numOfTicksAllowedForSteering);


#endif  //__FAPI_WLAN_BAND_STEERING_H_
