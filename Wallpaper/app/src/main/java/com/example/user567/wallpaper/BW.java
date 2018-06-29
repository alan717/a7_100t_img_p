package com.example.user567.wallpaper;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.service.wallpaper.WallpaperService;
import android.service.wallpaper.WallpaperService.Engine;
import android.view.SurfaceHolder;

//import javax.microedition.khronos.opengles.GL;

public abstract class BW extends WallpaperService {

    public class Eng extends Engine {
        final /* synthetic */ BW a;
        private GLSV b;
        private boolean c;

        class GLSV extends GLSurfaceView {
            final /* synthetic */ Eng a;

            GLSV(Eng aVar, Context context) {
                super(context);
                this.a = aVar;
            }

            public void GLSV() {
                super.onDetachedFromWindow();
            }

            public SurfaceHolder getHolder() {
                return this.a.getSurfaceHolder();
            }
        }

        public Eng(BW bVar) {
            //super(bVar);
            this.a = bVar;
        }

        protected void Eng(int i) {
            this.b.setEGLContextClientVersion(i);
        }

        protected void a(Renderer renderer) {
            this.b.setRenderer(renderer);
            this.c = true;
        }

        @Override
        public void onCreate(SurfaceHolder surfaceHolder) {
            super.onCreate(surfaceHolder);
            this.b = new GLSV(this, this.a);
        }

        public void onDestroy() {
            super.onDestroy();
            this.b.GLSV();
        }

        public void onVisibilityChanged(boolean z) {
            super.onVisibilityChanged(z);
            if (!this.c) {
                return;
            }
            if (z) {
                this.b.onResume();
            } else {
                this.b.onPause();
            }
        }
    }

}
