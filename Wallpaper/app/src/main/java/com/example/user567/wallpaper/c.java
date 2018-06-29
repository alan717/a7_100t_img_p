package com.example.user567.wallpaper;


import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.media.AudioManager;
import android.media.AudioManager.OnAudioFocusChangeListener;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.media.MediaPlayer.OnVideoSizeChangedListener;
import android.preference.PreferenceManager;
import android.util.DisplayMetrics;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.ScaleGestureDetector.SimpleOnScaleGestureListener;
import android.view.SurfaceHolder;
import java.io.File;
import java.io.IOException;

public class c extends BW {
    private f a;
    private int b;
    private int c;

    class Engg extends BW.Eng implements OnAudioFocusChangeListener {
        private int A = 0;
        private boolean B = false;
        private boolean C = true;
        private boolean D = false;
        private String E = "";
        private int F;
        private boolean G;
        private OnSharedPreferenceChangeListener H = new OnSharedPreferenceChangeListener() {
            @Override
            public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String str) {
                if (D) {
                    return;
                }
                switch (str) {
                    default: {
                    }
                    case "video_file": {
                        E = com.example.user567.wallpaper.c.Engg.this.e.b("video_file", "");
                        if (!com.example.user567.wallpaper.c.Engg.this.E.equalsIgnoreCase("") && new File(com.example.user567.wallpaper.c.Engg.this.E).exists()) {
                            com.example.user567.wallpaper.c.Engg.this.a(com.example.user567.wallpaper.c.Engg.this.E);
                            return;
                        }
                        com.example.user567.wallpaper.c.Engg.this.e.a(com.example.user567.wallpaper.c.Engg.this.e.e(), "", true);
                    }
                    case "mute": {
                        com.example.user567.wallpaper.c.Engg.this.p = com.example.user567.wallpaper.c.Engg.this.e.b("mute", false);
                        com.example.user567.wallpaper.c.Engg.this.a(com.example.user567.wallpaper.c.Engg.this.p);
                    }
                    case "rotate": {
                        com.example.user567.wallpaper.c.Engg.this.B = com.example.user567.wallpaper.c.Engg.this.e.b("rotate", false);
                        com.example.user567.wallpaper.c.Engg.this.h();
                    }
                    case "scale": {
                        com.example.user567.wallpaper.c.Engg.this.o = com.example.user567.wallpaper.c.Engg.this.e.b("scale", true);
                    }
                    case "doubleClick": {
                        com.example.user567.wallpaper.c.Engg.this.q = com.example.user567.wallpaper.c.Engg.this.e.b("doubleClick", true);
                    }
                    case "pref_carousel_type": {
                        com.example.user567.wallpaper.c.Engg.this.G = com.example.user567.wallpaper.c.Engg.this.e.b("pref_carousel_type", true);
                    }
                    case "pref_carousel_dir": {
                        com.example.user567.wallpaper.c.Engg.this.F = Integer.parseInt(com.example.user567.wallpaper.c.Engg.this.e.b("pref_carousel_dir", "8"));
                    }
                }
            }

        };
            e.a  b = new e.a() {
            @Override
            public void a() {
                if (com.example.user567.wallpaper.c.Engg.this.player != null && !com.example.user567.wallpaper.c.Engg.this.player.isPlaying()) {
                    com.example.user567.wallpaper.c.Engg.this.player.start();
                }
                if (com.example.user567.wallpaper.c.Engg.this.q) {
                    final Intent intent = new Intent(com.example.user567.wallpaper.c.Engg.this.d, (Class)MainActivity.class);
                    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    com.example.user567.wallpaper.c.Engg.this.d.startActivity(intent);
                }
            }

            @Override
            public void a( float n,  float n2) {
                if (!com.example.user567.wallpaper.c.Engg.this.o || com.example.user567.wallpaper.c.Engg.this.e.b() || com.example.user567.wallpaper.c.this.a == null) {
                    return;
                }
                com.example.user567.wallpaper.c.Engg.this.t -= n / com.example.user567.wallpaper.c.this.b * 2.0f;
                com.example.user567.wallpaper.c.Engg.this.u += n2 / com.example.user567.wallpaper.c.this.c * 2.0f;
                com.example.user567.wallpaper.c.Engg.this.h();
            }
        };
        final /* synthetic */ c c;
        private Context d;
        private d e;
        private MediaPlayer player;
        private int g = 0;
        private int h = 0;
        private int i = 0;
        private int j = 0;
        private float k = 1.0f;
        private ScaleGestureDetector l;
        private e m;
        private GestureDetector n;
        private boolean o = true;
        private boolean p = false;
        private boolean q = true;
        private float r = 1.0f;
        private float s = 1.0f;
        private float t = 0.0f;
        private float u = 0.0f;
        private float v = 1.0f;
        private float w = 1.0f;
        private float x = 1.0f;
        private float y = 1.0f;
        private int z = 0;

        private class listener extends SimpleOnScaleGestureListener {
            final /* synthetic */ Engg a;

            private listener(Engg aVar) {
                this.a = aVar;
            }


            public boolean onScale(ScaleGestureDetector scaleGestureDetector) {
                if (this.a.o && this.a.c.a != null) {
                    this.a.k = scaleGestureDetector.getScaleFactor();
                    this.a.a(this.a.r * this.a.k, this.a.s * this.a.k);
                    this.a.y = this.a.r / this.a.x;
                    this.a.h();
                }
                return true;
            }
        }

        Engg(c cVar) {
            super(cVar);
            this.c = cVar;
        }

        private void a() {
            if (this.player == null) {
                this.player = new MediaPlayer();
                this.c.a = new f(this.player);
                a(this.c.a);
                this.player.setOnPreparedListener(new OnPreparedListener() {

                    @Override
                    public void onPrepared(MediaPlayer mediaPlayer) {
                        com.example.user567.wallpaper.c.Engg.this.D = true;
                        com.example.user567.wallpaper.c.Engg.this.player.start();
                        com.example.user567.wallpaper.c.Engg.this.a(com.example.user567.wallpaper.c.Engg.this.p);
                    }
                });
                this.player.setOnCompletionListener(new OnCompletionListener() {


                    public void onCompletion(MediaPlayer mediaPlayer) {
                        com.example.user567.wallpaper.c.Engg.this.e.a("video_file_bak", com.example.user567.wallpaper.c.Engg.this.E);
                        if (com.example.user567.wallpaper.c.Engg.this.F >= 8) {
                            com.example.user567.wallpaper.c.Engg.this.player.start();
                        } else if (com.example.user567.wallpaper.c.Engg.this.e.f() > 0) {
                            com.example.user567.wallpaper.c.Engg.this.E = com.example.user567.wallpaper.c.Engg.this.e.b(com.example.user567.wallpaper.c.Engg.this.G);
                            com.example.user567.wallpaper.c.Engg.this.a(com.example.user567.wallpaper.c.Engg.this.E);
                        } else {
                            com.example.user567.wallpaper.c.Engg.this.player.start();
                        }
                    }
                });
                this.player.setOnVideoSizeChangedListener(new OnVideoSizeChangedListener() {


                    public void onVideoSizeChanged(MediaPlayer mediaPlayer, int i, int i2) {
                        com.example.user567.wallpaper.c.Engg.this.i = i;
                        com.example.user567.wallpaper.c.Engg.this.j = i2;
                        com.example.user567.wallpaper.c.Engg.this.h();
                    }
                });
                this.player.setOnErrorListener(new OnErrorListener() {


                    public boolean onError(MediaPlayer mediaPlayer, int i, int i2) {
                        com.example.user567.wallpaper.c.Engg.this.A = com.example.user567.wallpaper.c.Engg.this.A + 1;
                        if (com.example.user567.wallpaper.c.Engg.this.A > 4) {
                            com.example.user567.wallpaper.c.Engg.this.A = 0;
                            String b = com.example.user567.wallpaper.c.Engg.this.e.b("video_file_bak", "");
                            if (b.equalsIgnoreCase("")) {
                                com.example.user567.wallpaper.c.Engg.this.e.c(true);
                            } else {
                                com.example.user567.wallpaper.c.Engg.this.a(b);
                            }
                        }
                        return false;
                    }
                });
            }
            if (this.E.equalsIgnoreCase("")) {
                this.e.a(this.e.e(), "", true);
            } else {
                a(this.E);
            }
        }

        private void a(float f, float f2) {
            float f3 = f > f2 ? f2 : f;
            float f4 = f > f2 ? f : f2;
            if (f3 < 4.0f && f4 > 0.2f) {
                this.r = f;
                this.s = f2;
            }
        }

        private void a(String str) {
            try {
                if (this.D) {
                    this.player.reset();
                }
                this.player.setDataSource(str);
                this.player.prepare();
            } catch (IllegalStateException e) {
            } catch (IOException e2) {
            } catch (IllegalArgumentException e3) {
            } catch (SecurityException e4) {
            }
        }

        private void a(boolean z) {
            if (this.player == null) {
                return;
            }
            if (z) {
                this.player.setVolume(0.0f, 0.0f);
            } else {
                this.player.setVolume(1.0f, 1.0f);
            }
        }

        private void b() {
            ((AudioManager) this.d.getSystemService(Context.AUDIO_SERVICE)).requestAudioFocus(this, 3, 1);//audio
        }

        private void b(int i) {
            switch (i) {
                case 0:
                    e();
                    break;
                case 1:
                    f();
                    break;
                case 2:
                    this.s = 1.0f;
                    this.r = 1.0f;
                    break;
                default:
                    f();
                    break;
            }
            this.x = this.r;
        }

        private void c() {
            if (this.B) {
                this.h = this.i;
                this.g = this.j;
                this.c.a.a(-90);
            } else {
                this.h = this.j;
                this.g = this.i;
                this.c.a.a(0);
            }
            this.C = g();
        }

        private void d() {
            this.v = this.r > 1.0f ? this.r - 1.0f : 1.0f - this.r;
            this.w = this.s > 1.0f ? this.s - 1.0f : 1.0f - this.s;
            if (this.t > this.v) {
                this.t = this.v;
            } else if (this.t < (-this.v)) {
                this.t = -this.v;
            }
            if (this.u > this.w) {
                this.u = this.w;
            } else if (this.u < (-this.w)) {
                this.u = -this.w;
            }
        }

        private void e() {
            if (this.C) {
                this.s = 1.0f;
                this.r = (((float) this.g) * ((float) this.c.c)) / (((float) this.h) * ((float) this.c.b));
                return;
            }
            this.r = 1.0f;
            this.s = (((float) this.h) * ((float) this.c.b)) / (((float) this.g) * ((float) this.c.c));
        }

        private void f() {
            if (this.C) {
                this.r = 1.0f;
                this.s = (((float) this.h) * ((float) this.c.b)) / (((float) this.g) * ((float) this.c.c));
                return;
            }
            this.s = 1.0f;
            this.r = (((float) this.g) * ((float) this.c.c)) / (((float) this.h) * ((float) this.c.b));
        }

        private boolean g() {
            return this.g == 0 || (((float) this.c.b) / ((float) this.g)) * ((float) this.h) < ((float) this.c.c);
        }

        private void h() {
            c();
            b(this.z);
            this.r *= this.y;
            this.s *= this.y;
            d();
            this.c.a.a(this.y);
            this.c.a.a(this.t, this.u);
            if (this.B) {
                this.c.a.b(this.s, this.r);
            } else {
                this.c.a.b(this.r, this.s);
            }
        }

        public void onAudioFocusChange(int i) {
            switch (i) {
                case -1:
                    a(true);
                    return;
                case 1:
                    a(this.p);
                    return;
                default:
                    return;
            }
        }

        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);
            this.d = this.c.getApplicationContext();
            this.e = com.example.user567.wallpaper.d.getnew();
            PreferenceManager.getDefaultSharedPreferences(this.d).registerOnSharedPreferenceChangeListener(this.H);
            ((BW.Eng)this).Eng(2);
            DisplayMetrics displayMetrics = this.c.getResources().getDisplayMetrics();
            this.c.c = displayMetrics.heightPixels;
            this.c.b = displayMetrics.widthPixels;
            this.B = false;
            this.E = this.e.b("video_file", "");
            this.G = this.e.b("pref_carousel_type", true).booleanValue();
            this.F = Integer.parseInt(this.e.b("pref_carousel_dir", "8"));
            this.p = this.e.b("mute", false).booleanValue();
            this.o = this.e.b("scale", true).booleanValue();
            this.q = this.e.b("doubleClick", true).booleanValue();
            a();
            b();
            this.l = new ScaleGestureDetector(this.d, new listener(this) );
            this.m = new e(this.b);
            this.n = new GestureDetector(this.d, this.m);
        }

        public void onDestroy() {
            PreferenceManager.getDefaultSharedPreferences(this.d).unregisterOnSharedPreferenceChangeListener(this.H);
            this.e.a("carousel", false);
            this.e.a("rotate", false);
            this.player.release();
            this.player = null;
            super.onDestroy();
        }

        public void onTouchEvent(MotionEvent motionEvent) {
            if (!this.n.onTouchEvent(motionEvent) && !this.l.onTouchEvent(motionEvent)) {
                super.onTouchEvent(motionEvent);
            }
        }

        public void onVisibilityChanged(boolean z) {
            if (this.D) {
                if (z) {
                    if (!this.player.isPlaying()) {
                        this.player.start();
                    }
                } else if (this.player.isPlaying()) {
                    this.player.pause();
                }
            }
            super.onVisibilityChanged(z);
        }
    }

    public Engine onCreateEngine() {
        return new Engg(this);
    }
}