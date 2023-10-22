package com.unionpay.client3.tsm;

import android.os.Bundle;

interface ITsmConnectionCallback {

    void onResult(in Bundle result);
    void onError(String errorCode, String errorMessage);
}