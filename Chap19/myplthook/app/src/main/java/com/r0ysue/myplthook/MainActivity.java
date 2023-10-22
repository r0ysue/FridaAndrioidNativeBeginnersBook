package com.r0ysue.myplthook;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
   static String filesPath;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
filesPath=getApplication().getPackageCodePath();
        filesPath =filesPath.substring(0,filesPath.length()-8);
        filesPath=filesPath+"lib/arm64/libnative-lib.so";
        // Example of a call to a native method
        Log.e("r0ysue",filesPath);
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}