package com.unionpay.client3.tsm;

import android.os.Bundle;

public interface ITsmTransCallback {

    void onSuccess(int seType, int request, Bundle bundle);

    void onFailed(int seType, int request, String errorCode, String errorMessage);

}
