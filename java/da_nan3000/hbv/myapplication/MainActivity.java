package da_nan3000.hbv.myapplication;

import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.webkit.WebView;

import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;

import android.support.v4.app.ActivityCompat;
import android.Manifest;

public class MainActivity extends android.app.Activity  {

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    @Override
    public void onCreate(android.os.Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        android.webkit.WebView wv = new android.webkit.WebView(this);
        setContentView(wv);

        ActivityCompat.requestPermissions((Activity) this, new String[] {Manifest.permission.READ_CONTACTS}, 1);

        wv.getSettings().setAllowFileAccessFromFileURLs(true);
        wv.getSettings().setAllowUniversalAccessFromFileURLs(true);
        wv.getSettings().setJavaScriptEnabled(true);
        wv.getSettings().getAllowContentAccess();
        wv.getSettings().setBuiltInZoomControls(false);
        wv.addJavascriptInterface(new JavaScriptGrensesnitt(this), "javascriptGrensesnitt");
        wv.loadUrl("file:///android_asset/index.html");


    }
}