package com.roysue.r0ysue1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import java.lang.reflect.Method;

import dalvik.system.DexClassLoader;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        DexClassLoader loader=new DexClassLoader("/sdcard/3.dex","/sdcard","/sdcard",this.getClassLoader());
        try {
            Class Main=loader.loadClass("com.roysue.mydex.r1ysue");
            for(Method a : Main.getDeclaredMethods()){
                if(a.getName().indexOf("add")>=0)
                    a.invoke(null,1,2);


            }


        } catch (Exception e) {
            e.printStackTrace();
        }

    }
}