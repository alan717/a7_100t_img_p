package com.example.user567.wallpaper;

import android.app.Application;

public class MyApplication extends Application {
    private static MyApplication a;

    public static MyApplication a() {
        return a;
    }

    public void onCreate() {
        super.onCreate();
        a = this;
    }
}
