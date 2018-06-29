package com.example.user567.wallpaper;



import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.media.MediaPlayer;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView.Renderer;
import android.opengl.Matrix;
import android.view.Surface;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class f implements OnFrameAvailableListener, Renderer {
    private static String a = "GLSurfaceRenderer";
    private MediaPlayer b;
    private SurfaceTexture c;
    private int d = 0;
    private int e = 0;
    private int f = 0;
    private int g = 0;
    private int h = 0;
    private int i = 0;
    private boolean j = false;
    private FloatBuffer k;
    private FloatBuffer l;
    private final float[] m = new float[]{0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f};
    private int n = 0;
    private boolean o;
    private float[] p = new float[16];
    private float[] q = new float[16];
    private final String r = "uniform mat4 uSTMatrix;\nuniform mat4 uMVPMatrix;\nattribute vec4 position;\nattribute vec4 texCoords;\nvarying vec2 outTexCoords;\nvoid main(void) {\n    outTexCoords = (uSTMatrix * texCoords).xy;\n    gl_Position = uMVPMatrix * position;\n}\n";
    private final String s = "#extension GL_OES_EGL_image_external : require\nprecision mediump float;\nvarying vec2 outTexCoords;\nuniform samplerExternalOES texture;\nvoid main(void) {\n    gl_FragColor = texture2D(texture, outTexCoords);\n}\n";

    public f(MediaPlayer mediaPlayer) {
        this.b = mediaPlayer;
        Matrix.setIdentityM(this.q, 0);
        this.l = ByteBuffer.allocateDirect(this.m.length * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
        this.l.put(this.m).position(0);
        Matrix.setIdentityM(this.p, 0);
        if (!this.o) {
            this.o = true;
            b(0.001f, 0.001f);
        }
    }

    private int a(int i, String str) {
        int glCreateShader = GLES20.glCreateShader(i);
        if (glCreateShader != 0) {
            GLES20.glShaderSource(glCreateShader, str);
            GLES20.glCompileShader(glCreateShader);
            int[] iArr = new int[1];
            GLES20.glGetShaderiv(glCreateShader, 35713, iArr, 0);
            if (iArr[0] == 0) {
                GLES20.glDeleteShader(glCreateShader);
                return 0;
            }
        }
        return glCreateShader;
    }

    private int a(String str, String str2) {
        int a = a(35633, str);
        int a2 = a(35632, str2);
        if (a == 0 || a2 == 0) {
            return 0;
        }
        int glCreateProgram = GLES20.glCreateProgram();
        if (glCreateProgram != 0) {
            GLES20.glAttachShader(glCreateProgram, a);
            a("glAttachShader");
            GLES20.glAttachShader(glCreateProgram, a2);
            a("glAttachShader");
            GLES20.glLinkProgram(glCreateProgram);
            int[] iArr = new int[1];
            GLES20.glGetProgramiv(glCreateProgram, 35714, iArr, 0);
            if (iArr[0] != 1) {
                GLES20.glDeleteProgram(glCreateProgram);
                return 0;
            }
        }
        return glCreateProgram;
    }

    private int a(GL10 gl10, EGLConfig eGLConfig) {
        int[] iArr = new int[1];
        GLES20.glGenTextures(1, iArr, 0);
        int i = iArr[0];
        GLES20.glBindTexture(36197, i);
        GLES20.glTexParameterf(36197, 10241, 9728.0f);
        GLES20.glTexParameterf(36197, 10240, 9729.0f);
        this.c = new SurfaceTexture(i);
        this.c.setOnFrameAvailableListener(this);
        Surface surface = new Surface(this.c);
        try {
            this.b.setSurface(surface);
        } catch (Exception e) {
        }
        surface.release();
        return i;
    }

    private void a(String str) {
        int glGetError = GLES20.glGetError();
        if (glGetError != 0) {
            throw new RuntimeException(str + ": glError " + glGetError);
        }
    }

    public void a() {
        GLES20.glUseProgram(this.d);
        GLES20.glActiveTexture(33984);
        GLES20.glBindTexture(36197, this.e);
        GLES20.glVertexAttribPointer(this.f, 3, 5126, false, 0, this.k);
        GLES20.glEnableVertexAttribArray(this.f);
        GLES20.glVertexAttribPointer(this.g, 4, 5126, false, 0, this.l);
        GLES20.glEnableVertexAttribArray(this.g);
        GLES20.glUniformMatrix4fv(this.h, 1, false, this.p, 0);
        GLES20.glUniformMatrix4fv(this.i, 1, false, this.q, 0);
        GLES20.glDrawArrays(6, 0, 4);
        GLES20.glFinish();
    }

    public void a(float f) {
        Matrix.setIdentityM(this.q, 0);
        Matrix.scaleM(this.q, 0, f, f, 0.0f);
    }

    public void a(float f, float f2) {
        Matrix.setIdentityM(this.q, 0);
        Matrix.translateM(this.q, 0, f, f2, 0.0f);
    }

    public void a(int i) {
        this.n = i;
    }

    public void b(float f, float f2) {
        float[] fArr = new float[]{-f, f2, 0.0f, -f, -f2, 0.0f, f, -f2, 0.0f, f, f2, 0.0f};
        this.k = ByteBuffer.allocateDirect(fArr.length * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
        this.k.put(fArr).position(0);
        Matrix.rotateM(this.q, 0, (float) this.n, 0.0f, 0.0f, 1.0f);
    }

    public void onDrawFrame(GL10 gl10) {
        synchronized (this) {
            if (this.j) {
                try {
                    this.c.updateTexImage();
                } catch (Exception e) {
                }
                this.c.getTransformMatrix(this.p);
                this.j = false;
            }
        }
        GLES20.glClearColor(0.34f, 0.59f, 0.6f, 0.0f);
        GLES20.glClear(16640);
        a();
    }

    public synchronized void onFrameAvailable(SurfaceTexture surfaceTexture) {
        this.j = true;
    }

    public void onSurfaceChanged(GL10 gl10, int i, int i2) {
    }

    public void onSurfaceCreated(GL10 gl10, EGLConfig eGLConfig) {
        this.d = a("uniform mat4 uSTMatrix;\nuniform mat4 uMVPMatrix;\nattribute vec4 position;\nattribute vec4 texCoords;\nvarying vec2 outTexCoords;\nvoid main(void) {\n    outTexCoords = (uSTMatrix * texCoords).xy;\n    gl_Position = uMVPMatrix * position;\n}\n", "#extension GL_OES_EGL_image_external : require\nprecision mediump float;\nvarying vec2 outTexCoords;\nuniform samplerExternalOES texture;\nvoid main(void) {\n    gl_FragColor = texture2D(texture, outTexCoords);\n}\n");
        if (this.d != 0) {
            this.f = GLES20.glGetAttribLocation(this.d, "position");
            a("glGetAttribLocation");
            if (this.f == -1) {
                throw new RuntimeException("Could not get attrib location for position");
            }
            this.g = GLES20.glGetAttribLocation(this.d, "texCoords");
            this.h = GLES20.glGetUniformLocation(this.d, "uSTMatrix");
            this.i = GLES20.glGetUniformLocation(this.d, "uMVPMatrix");
            this.e = a(gl10, eGLConfig);
            synchronized (this) {
                this.j = false;
            }
        }
    }
}
