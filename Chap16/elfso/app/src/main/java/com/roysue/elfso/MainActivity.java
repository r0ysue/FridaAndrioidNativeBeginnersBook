package com.roysue.elfso;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        TextView textview;
        Button button=findViewById(R.id.button);
//        try {
//            Thread.sleep(50*1000);
//        } catch (InterruptedException e) {
//            e.printStackTrace();
//        }

//dddddd(1);
        int a=add(1,2);
 Log.e("r0ysue", String.valueOf(a));

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native int add(int a,int b);
    public native int dddddd(int s);
//    public native void sd();

}