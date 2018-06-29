package com.example.user567.wallpaper;

import android.content.ContentUris;
import android.content.Context;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Build;
import android.os.Build.VERSION;
import android.os.Environment;
import android.provider.DocumentsContract;
import android.provider.MediaStore;
import android.provider.MediaStore.Audio;
import android.provider.MediaStore.Images.Media;
import android.provider.MediaStore.Video;

public class first {
    public static Intent a() {
        Intent intent = new Intent("android.intent.action.GET_CONTENT");
        intent.setType("video/*");
        intent.addCategory("android.intent.category.OPENABLE");
        return intent;
    }

    public static String a(Context context, Uri uri) {
        final Uri uri2 = null;
        String string = null;
        boolean b;
        if (Build.VERSION.SDK_INT >= 19) {
            b = true;
        }
        else {
            b = false;
        }
        if (b && DocumentsContract.isDocumentUri(context, uri)) {
            if (a(uri)) {
                final String[] split = DocumentsContract.getDocumentId(uri).split(":");
                if ("primary".equalsIgnoreCase(split[0])) {
                    string = Environment.getExternalStorageDirectory() + "/" + split[1];
                }
            }
            else {
                if (b(uri)) {
                    return a(context, ContentUris.withAppendedId(Uri.parse("content://downloads/public_downloads"), (long)Long.valueOf(DocumentsContract.getDocumentId(uri))), null, null);
                }
                if (c(uri)) {
                    final String[] split2 = DocumentsContract.getDocumentId(uri).split(":");
                    final String s = split2[0];
                    if ("image".equals(s)) {
                        uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                    }
                    else if ("video".equals(s)) {
                        uri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                    }
                    else {
                        uri = uri2;
                        if ("audio".equals(s)) {
                            uri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                        }
                    }
                    return a(context, uri, "_id=?", new String[] { split2[1] });
                }
            }
        }
        else if ("content".equalsIgnoreCase(uri.getScheme())) {
            if (d(uri)) {
                return uri.getLastPathSegment();
            }
            return a(context, uri, null, null);
        }
        else if ("file".equalsIgnoreCase(uri.getScheme())) {
            return uri.getPath();
        }
        return string;
    }

    public static String a(Context context, Uri uri, String str, String[] strArr) {
        Throwable th;
        Cursor cursor = null;
        String str2 = "_data";
        try {
            Cursor query = context.getContentResolver().query(uri, new String[]{"_data"}, str, strArr, null);
            if (query != null) {
                try {
                    if (query.moveToFirst()) {
                        str2 = query.getString(query.getColumnIndexOrThrow("_data"));
                        if (query == null) {
                            return str2;
                        }
                        query.close();
                        return str2;
                    }
                } catch (Throwable th2) {
                    th = th2;
                    cursor = query;
                    if (cursor != null) {
                        cursor.close();
                    }
                    throw th;
                }
            }
            if (query != null) {
                query.close();
            }
            return null;
        } catch (Throwable th3) {
            th = th3;
            if (cursor != null) {
                cursor.close();
            }
            throw new IllegalStateException("eeror ");
        }
    }

    public static boolean a(Uri uri) {
        return "com.android.externalstorage.documents".equals(uri.getAuthority());
    }

    public static boolean b(Uri uri) {
        return "com.android.providers.downloads.documents".equals(uri.getAuthority());
    }

    public static boolean c(Uri uri) {
        return "com.android.providers.media.documents".equals(uri.getAuthority());
    }

    public static boolean d(Uri uri) {
        return "com.google.android.apps.photos.content".equals(uri.getAuthority());
    }
}
