package com.example.user567.wallpaper;



import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;

public class e extends SimpleOnGestureListener {
    private a a;

    public interface a {
        void a();

        void a(float f, float f2);
    }

    public e(a aVar) {
        this.a = aVar;
    }

    public boolean onDoubleTap(MotionEvent motionEvent) {
        this.a.a();
        return false;
    }

    public boolean onDown(MotionEvent motionEvent) {
        return true;
    }

    public boolean onScroll(MotionEvent motionEvent, MotionEvent motionEvent2, float f, float f2) {
        this.a.a(f, f2);
        return false;
    }

    public boolean onSingleTapUp(MotionEvent motionEvent) {
        return false;
    }
}
