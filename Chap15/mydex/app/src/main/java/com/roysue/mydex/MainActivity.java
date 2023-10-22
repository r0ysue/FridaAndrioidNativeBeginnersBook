package com.roysue.mydex;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import dalvik.system.DexClassLoader;

public class MainActivity extends AppCompatActivity implements r0ysue {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        try {
            Thread.sleep(50*1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.load("/data/local/tmp/reg.so");
//        add(1,2);
//        DexClassLoader loader=new DexClassLoader("/sdcard/2.dex","/sdcard","/sdcard",this.getClassLoader());

    }

public int add(int a,int b){
Exception e=new Exception("i am trace");
e.printStackTrace();
        return a+b;
}
    static final int sl=100;
    static int r0ysuex=100;
}