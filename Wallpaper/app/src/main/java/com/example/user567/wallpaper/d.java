package com.example.user567.wallpaper;

import android.app.Activity;
import android.app.PendingIntent;
import android.app.WallpaperInfo;
import android.app.WallpaperManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.Uri;
import android.os.Build.VERSION;
import android.os.Environment;
import android.preference.PreferenceManager;
import android.service.wallpaper.WallpaperService;
import android.support.annotation.Nullable;

import java.io.File;
import java.io.FileFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;



public class d {

    private static d c;
    private SharedPreferences a;
    private Context b = MyApplication.a().getApplicationContext();
    private boolean d = false;
    private String e;
    private boolean f = false;
    private int g = 0;
    private int h = 0;
    private List<String> i = new ArrayList();

    private d() {
    }

    public static d getnew() {
        if (c == null) {
            synchronized (d.class) {
                if (c == null) {
                    c = new d();
                }
            }
        }
        return c;
    }

    private void a(final List<String> list, File file, final String str, final boolean z) {
        file.listFiles(new FileFilter( ) {

            public boolean accept(File file) {
                String str = "";
                String name = file.getName();
                int indexOf = name.indexOf(46);
                if (!z || str.contains(".danmaku.")) {
                    if (com.example.user567.wallpaper.d.this.h > 100) {
                        indexOf = -1;
                    }
                } else if (com.example.user567.wallpaper.d.this.h > 0) {
                    indexOf = -1;
                }
                if (indexOf != -1) {
                    str = name.substring(indexOf + 1);
                    if (com.example.user567.wallpaper.d.this.b(str)) {
                        String absolutePath = file.getAbsolutePath();
                        if (!str.equals("") && absolutePath.equals(str)) {
                            com.example.user567.wallpaper.d.this.g = com.example.user567.wallpaper.d.this.h;
                        }
                        com.example.user567.wallpaper.d.this.h = com.example.user567.wallpaper.d.this.h + 1;
                        list.add(absolutePath);
                        return true;
                    } else if (str.contains(".bili2api.")) {
                        com.example.user567.wallpaper.d.this.a(list, file, str, z);
                        return false;
                    }
                } else if (str.contains("aHYshipin")) {
                    str = file.getAbsolutePath();
                    if (str.contains("formatss")) {
                        return false;
                    }
                    com.example.user567.wallpaper.d.this.h = com.example.user567.wallpaper.d.this.h + 1;
                    list.add(str);
                    return true;
                } else if (file.isDirectory() && z && (com.example.user567.wallpaper.d.this.h == 0 || str.contains(".danmaku."))) {
                    com.example.user567.wallpaper.d.this.a(list, file, str, z);
                } else {
                    str = file.getAbsolutePath();
                    if (str.contains("Acfun/core/local") && name.length() < 3) {
                        com.example.user567.wallpaper.d.this.h = com.example.user567.wallpaper.d.this.h + 1;
                        list.add(str);
                        return true;
                    }
                }
                return false;
            }
        });
    }

    private boolean b(String str) {
        return str.equalsIgnoreCase("mp4") || str.equalsIgnoreCase("flv") || str.equalsIgnoreCase("blv") || str.equalsIgnoreCase("kgtmp") || str.equalsIgnoreCase("avi") || str.equalsIgnoreCase("wmv") || str.equalsIgnoreCase("asf") || str.equalsIgnoreCase("webm") || str.equalsIgnoreCase("mkv") || str.equalsIgnoreCase("ts") || str.equalsIgnoreCase("m2ts") || str.equalsIgnoreCase("m4v") || str.equalsIgnoreCase("3gp") || str.equalsIgnoreCase("3g2");
    }

    public void a(String str, String str2) {
        Editor edit = d().edit();
        edit.putString(str, str2);
        edit.apply();
    }

    public void a(String str, boolean z) {
        Editor edit = d().edit();
        edit.putBoolean(str, z);
        edit.apply();
    }

    public void a(boolean z) {
        com.example.user567.wallpaper.d.this.d = z;
    }

    public boolean a(int i) {
        switch (i) {
            case 0:
                String b = b("video_file", "");
                return !b.equalsIgnoreCase("") ? a(new File(b).getParent(), "", false) : false;
            case 1:
                return a(e() + "/aHYshipin", "aHYshipin", false);
            case 2:
                return a(e() + "/avideoshare/mp4preview/livewallaper", "", false);
            case 3:
                return a(e() + "/Android/data/tv.danmaku.bili/download", ".danmaku.", true);
            case 4:
                return a(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getPath() + "/Camera", "", false);
            case 5:
                return a("/Acfun/core/local", ".danmaku.", true);
            case 6:
                return a(e() + "/YinyuetaiVideo", "com.ss.android.ugc.live", false);
            case 7:
                return a(e() + "/kugou/mv", ".danmaku.", true);
            default:
                return false;
        }
    }

    public boolean a(String str) {
        if (str == null || str.equalsIgnoreCase("")) {
            return false;
        }
        int lastIndexOf = str.lastIndexOf(".");
        return lastIndexOf > 0 && b(str.substring(lastIndexOf + 1));
    }

    public boolean a(String str, Activity activity) {
        Intent intent = new Intent();
        intent.setData(Uri.parse("mqqopensdkapi://bizAgent/qm/qr?url=http%3A%2F%2Fqm.qq.com%2Fcgi-bin%2Fqm%2Fqr%3Ffrom%3Dapp%26p%3Dandroid%26k%3D" + str));
        try {
            activity.startActivity(intent);
            return true;
        } catch (Exception e) {
            return false;
        }
    }

    public boolean a(String str, String str2, boolean z) {
        if (str == null) {
            return false;
        }
        this.i.clear();
        this.g = 0;
        this.h = 0;
        a(this.i, new File(str), str2, z);
        if (this.i.size() < 1) {
            return false;
        }
        a("video_file", (String) this.i.get(this.g));
        return true;
    }

    public Boolean b(String str, boolean z) {
        return Boolean.valueOf(d().getBoolean(str, z));
    }

    public String b(String str, String str2) {
        return d().getString(str, str2);
    }

    public String b(boolean z) {
        int size = this.i.size();
        if (size < 1) {
            return "";
        }
        String str = "";
        if (z) {
            this.g = new Random().nextInt(size);
            try {
                return (String) this.i.get(this.g);
            } catch (Exception e) {
                return str;
            }
        }
        this.g++;
        if (this.g == size) {
            this.g = 0;
        }
        try {
            return (String) this.i.get(this.g);
        } catch (Exception e2) {
            return str;
        }
    }

    public boolean b() {
        return this.d;
    }

    public void c(boolean z) {
        try {
            WallpaperManager.getInstance(this.b).clear();
        } catch (IOException e) {
            e.printStackTrace();
        }
        if (z) {
            g();
        }
        System.exit(0);
    }

    public boolean c() {
        WallpaperInfo wallpaperInfo = WallpaperManager.getInstance(this.b).getWallpaperInfo();
        if (wallpaperInfo != null) {
            if (wallpaperInfo.getServiceName().equals(this.b.getPackageName() + ".WallpaperService")) {
                return true;
            }
        }
        return false;
    }

    public SharedPreferences d() {
        if (this.a == null) {
            this.a = PreferenceManager.getDefaultSharedPreferences(this.b);
        }
        return this.a;
    }

    public String e() {
        if (this.e == null) {
            this.e = Environment.getExternalStorageDirectory().getPath();
        }
        return this.e;
    }

    public int f() {
        return this.i == null ? -1 : this.i.size();
    }

    public void g() {
        Intent intent = new Intent();
        if (VERSION.SDK_INT >= 16) {
            intent.setAction("android.service.wallpaper.CHANGE_LIVE_WALLPAPER");
            intent.putExtra("android.service.wallpaper.extra.LIVE_WALLPAPER_COMPONENT", new ComponentName(this.b, WallpaperService.class));
        } else {
            intent.setAction("android.service.wallpaper.LIVE_WALLPAPER_CHOOSER");
        }
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);//268435456
        this.b.startActivity(intent);
    }
}