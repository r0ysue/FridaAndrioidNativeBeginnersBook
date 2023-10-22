package com.unionpay.client3.tsm;

import com.unionpay.client3.tsm.SeAppInfo;

interface ITsmConnection {

    String sendApdu(int seType, String hexApdu, int channel);
    void closeChannel(int seType, int channel);

    SeAppInfo[] getSeApps(int seType);

}