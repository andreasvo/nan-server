package da_nan3000.hbv.myapplication;

import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;

public class MainActivity extends android.app.Activity  {

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN)
    @Override
    public void onCreate(android.os.Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        android.webkit.WebView wv = new android.webkit.WebView(this);
        setContentView(wv);

        wv.addJavascriptInterface(new JavaScriptGrensesnitt(this), "javascriptGrensesnitt");
        wv.getSettings().setAllowFileAccessFromFileURLs(true);
        wv.getSettings().setAllowUniversalAccessFromFileURLs(true);

        wv.getSettings().setJavaScriptEnabled(true);
        wv.getSettings().getAllowContentAccess();
        wv.getSettings().setBuiltInZoomControls(true);
        wv.loadUrl("file:///android_asset/index.html");
    }
}