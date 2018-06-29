package com.example.user567.wallpaper;


import android.app.Activity;
//import android.
import android.app.Fragment;
import android.app.WallpaperManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceScreen;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.FrameLayout;
import android.widget.Toast;


import java.io.File;


public class MainActivity extends Activity {
    private d a;

    public static class frage extends PreferenceFragment implements SharedPreferences.OnSharedPreferenceChangeListener {
        private d a;//待解决
        private boolean b = true;
        private Preference c;
        private Preference d;
        private ListPreference e;
        private String f = "";
        private int g;

        private void a() {
            startActivityForResult(Intent.createChooser(first.a(), getResources().getString(R.string.select_video)), 111);
        }

        private void a(String str) {
            if (str.equalsIgnoreCase("")) {
                this.c.setSummary("");
                return;
            }
            String[] split = str.split("/");
            this.c.setSummary("文件:" + split[split.length - 1]);
        }

        private void a(boolean z) {
            Drawable drawable = WallpaperManager.getInstance(getActivity()).getDrawable();
            FrameLayout frameLayout = (FrameLayout) getActivity().findViewById(android.R.id.content);
            if (z) {
                frameLayout.setBackground(null);
            } else if (Build.VERSION.SDK_INT >= 16) {
                frameLayout.setBackground(drawable);
            } else {
                this.a.a("theme_Translucent", true);
            }
        }

        private void b() {
            String b = this.a.b("video_file", "");
            if (this.g < 8) {
                this.d.setEnabled(true);
                if (this.a.f() <= 1) {
                    a(b);
                    return;
                } else if (!b.equalsIgnoreCase("")) {
                    String[] split = new File(b).getParent().split("/");
                    this.c.setSummary("轮播目录:" + split[split.length - 1] + "[共有" + this.a.f() + "个视频]");
                    return;
                } else {
                    return;
                }
            }
            this.d.setEnabled(false);
            a(b);
        }

        @Override
        public void onActivityResult(int i, int i2, Intent intent) {
            switch (i) {
                case 111:
                    if (i2 == -1) {
                        String a = first.a(getActivity(), intent.getData());
                        if (this.a.a(a)) {
                            this.a.a("video_file", a);
                            a(a);
                            this.a.a("carousel", "8");
                            return;
                        }
                        return;
                    }
                    return;
                default:
                    return;
            }
        }

        @Override
        public void onCreate(Bundle bundle) {
            super.onCreate(bundle);
            addPreferencesFromResource(R.xml.settings);
            this.a = com.example.user567.wallpaper.d.getnew();
            this.a.a(true);
            this.c = findPreference("select_file");
            this.e = (ListPreference) findPreference("pref_carousel_dir");
            this.d = findPreference("pref_carousel_type");
            this.b = this.a.b("theme_Translucent", true).booleanValue();
            a(this.b);
            Preference findPreference = findPreference("close");
            if (this.a.c()) {
                findPreference.setTitle("重启壁纸");
                findPreference.setSummary("发生错误或需清理内存时使用");
            } else {
                findPreference.setTitle("设置壁纸");
                findPreference.setSummary("将视频设置为动态壁纸");
            }
            this.g = Integer.parseInt(this.a.b("pref_carousel_dir", "0"));
            this.e.setSummary(this.e.getEntry());
            b();
        }

        @Override
        public void onDestroy() {
            this.a.a(false);
            super.onDestroy();
        }

        @Override
        public void onPause() {
            getPreferenceManager().getSharedPreferences().unregisterOnSharedPreferenceChangeListener(this);
            super.onPause();
        }

        @Override
        public boolean onPreferenceTreeClick(PreferenceScreen preferenceScreen, Preference preference) {
            boolean b = true;
            final String key = preference.getKey();
            switch (key) {
                case "select_file": {
                    this.a();
                    break;
                }
                case "rotate": {
                    final boolean booleanValue = this.a.b("rotate", false);
                    final d a = this.a;
                    if (booleanValue) {
                        b = false;
                    }
                    a.a("rotate", b);
                    break;
                }
                case "qq": {
                    this.a.a("7o8b4X_1kvkyKbmd3DSJbv3QSeBxhkKM", this.getActivity());
                    break;
                }
                case "hydrogen": {
                    this.getActivity().startActivity(new Intent("android.intent.action.VIEW", Uri.parse("http://www.coolapk.com/apk/com.hydrogen.video.lwp")));
                    break;
                }
                case "close": {
                    this.getActivity().finish();
                    this.a.c(true);
                    break;
                }
            }
            return super.onPreferenceTreeClick(preferenceScreen, preference);
        }

        @Override
        public void onResume() {
            getPreferenceManager().getSharedPreferences().registerOnSharedPreferenceChangeListener(this);
            super.onResume();
        }

        @Override
        public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String str) {
            switch (str) {
                default: {
                }
                case "theme_Translucent": {
                    this.a(this.b = sharedPreferences.getBoolean("theme_Translucent", true));
                }
                case "video_file": {
                    this.b();
                }
                case "pref_carousel_dir": {
                    this.g = Integer.parseInt(this.a.b("pref_carousel_dir", "8"));
                    if (this.g < 8) {
                        if (this.a.a(this.g)) {
                            this.e.setSummary(this.e.getEntry());
                        } else if (this.g != 0) {
                            Toast.makeText((Context) this.getActivity(), (CharSequence) "\u8be5\u76ee\u5f55\u65e0\u89c6\u9891!", Toast.LENGTH_SHORT).show();
                        }
                        this.d.setEnabled(true);
                        return;
                    }
                    this.a(this.a.b("video_file", ""));
                    this.e.setSummary(this.e.getEntry());
                    this.d.setEnabled(false);
                }
            }
        }
    }


    private void a() {
        finish();
        this.a.c(true);
    }

    @Override
    protected void onActivityResult(int i, int i2, Intent intent) {
        super.onActivityResult(i, i2, intent);
        finish();
    }

    @Override
//    protected void onCreate(Bundle bundle) {
//        super.onCreate(bundle);
//        this.a = com.example.user567.wallpaper.d.getnew();
//        Uri data = getIntent().getData();
//        if (data != null) {
//            String a = first.a(this, data);
//            if (this.a.a(a)) {
//                this.a.a("video_file", a);
//                this.a.a("pref_carousel_dir", "8");
//                a();
//            } else {
//                Toast.makeText(this, "不支持的视频格式", Toast.LENGTH_SHORT).show();
//                finish();
//            }
//        } else if (!(this.a.c() || this.a.b("video_file", "").equalsIgnoreCase(""))) {
//            a();
//        }
//        getWindow().setFlags(1024, 1024);
//
//        getFragmentManager().beginTransaction().replace(android.R.id.content, new frage()).commit();//16908290 真愉快;
//    }
    protected void onCreate(final Bundle bundle) {
        super.onCreate(bundle);
        this.a = d.getnew();
        final Uri data = this.getIntent().getData();
        if (data != null) {
            final String a = first.a((Context)this, data);
            if (this.a.a(a)) {
                this.a.a("video_file", a);
                this.a.a("pref_carousel_dir", "8");
                this.a();
            }
            else {
                Toast.makeText((Context)this, (CharSequence)"\u4e0d\u652f\u6301\u7684\u89c6\u9891\u683c\u5f0f", Toast.LENGTH_SHORT).show();
                this.finish();
            }
        }
        else if (!this.a.c() && !this.a.b("video_file", "").equalsIgnoreCase("")) {
            this.a();
        }
        this.getWindow().setFlags(1024, 1024);
        this.getFragmentManager().beginTransaction().replace(android.R.id.content, (Fragment)new frage()).commit();
    }

    @Override
    public void onDestroy() {
        getPackageManager().clearPackagePreferredActivities(getPackageName());
        super.onDestroy();
    }


}
