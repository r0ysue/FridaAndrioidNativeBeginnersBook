package com.r0ysue.r0encryptdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
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

        Button btn_md5 = findViewById(R.id.btn_md5);
        Button btn_saltmd5 = findViewById(R.id.btn_saltMd5);
        Button btn_halfmd5 = findViewById(R.id.btn_md5_half);
        Button btn_sha1 = findViewById(R.id.btn_sha1);
        Button btn_sha256 = findViewById(R.id.btn_sha256);
        Button btn_sha512 = findViewById(R.id.btn_sha512);
        Button btn_hmacmd5 = findViewById(R.id.btn_hmacMd5);
        Button btn_hmacsha1 = findViewById(R.id.btn_hmacSha1);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        btn_md5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(md5("123456"));
            }
        });


        btn_sha1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(sha1("123456"));
            }
        });

        btn_sha256.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(sha256("123456"));
            }
        });

        btn_sha512.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(sha512("123456"));
            }
        });

        btn_hmacmd5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(hmacmd5("123456","12345"));
            }
        });

        btn_hmacsha1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(hmacsha1("123456","12345"));
            }
        });

        btn_saltmd5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(saltmd5("123456"));
            }
        });

        btn_halfmd5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                tv.setText(md5half("123456"));
            }
        });
    }


    public native String stringFromJNI();

    public native String md5(String str);
    public native String md5half(String str);
    public native String saltmd5(String str);
    public native String sha1(String str);
    public native String sha256(String str);
    public native String sha512(String str);
    public native String hmacmd5(String str, String key);
    public native String hmacsha1(String str, String key);
}